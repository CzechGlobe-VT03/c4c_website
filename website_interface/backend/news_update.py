import gspread
from oauth2client.service_account import ServiceAccountCredentials
from datetime import datetime, timedelta
from os.path import join, isfile
import os
import json
import requests
from mimetypes import guess_extension


def _authorize_get_sheet(creds_file: str,  # | os.PathLike,
                         sheet_id: str) -> gspread.worksheet.Worksheet:
    # Define the scope
    scope: list[str] = ["https://spreadsheets.google.com/feeds",
                        "https://www.googleapis.com/auth/drive.readonly"]
    # Add your service account credentials file
    creds = ServiceAccountCredentials.from_json_keyfile_name(
        creds_file, scope)
    print('Is creds token valid:', not creds.access_token_expired)
    # Authorize the client
    client: gspread.client.Client = gspread.authorize(creds)
    # return the first sheet from the correct file
    return client.open_by_key(sheet_id).sheet1


def _get_latest_local(in_path: str) -> datetime:
    """Gets the datetime of the latest locally-saved news item."""
    with open(in_path, "r", encoding="utf-8") as file:
        data = json.load(file)
    return datetime.strptime(data[0]['timeStamp'], '%d.%m.%Y %H:%M:%S')


def _get_latest_gsheets(sheet: gspread.worksheet.Worksheet) -> bool:
    "Gets the datetime of the latest gsheets-saved news item."
    # Load the sheet and read the time of last response (last row, first col)
    row_count: int = len(sheet.col_values(1))
    last_edit_str: str = sheet.cell(row_count, 1).value

    # Convert time string to datetime and compare to time since last update
    last_edit = datetime.strptime(last_edit_str, '%d.%m.%Y %H:%M:%S')
    return last_edit


def _check_new_news(sheet: gspread.worksheet.Worksheet, local_path: str
                    ) -> bool:
    """Is there a new gsheets item, which is not saved locally yet?"""
    latest_local: datetime = _get_latest_local(local_path)
    latest_gsheets: datetime = _get_latest_gsheets(sheet)

    print(f'Last local news: {latest_local}')
    print(f'Last gsheets news: {latest_gsheets}')
    return latest_local != latest_gsheets


def _get_new_data(sheet: gspread.worksheet.Worksheet,
                  num_news: int = 0) -> list[dict[str, str]]:
    if num_news == 0:
        return sheet.get_all_records()
    else:
        return sheet.get_records(first_index=num_news+1)


def _parse_new(old_list: list[dict[str, str]],
               filenames_list: list[str]) -> list[dict[str, str]]:
    new_list: list[dict[str, str]] = []
    for idx, data in enumerate(old_list):
        new_list.append({
            'eventDate': data['Date of the event'],
            'eventTypes': data['Types of event'],
            'eventTitle': data['News title (max length 10 words)'],
            'eventDesc': data['Longer description of the event (recommended \
length 100-200 words)'],
            'eventLink': data['Link with more on the story (optional)'],
            'imgFilename': filenames_list[idx],
            'countries': data['Countries'],
            'timeStamp': data['Timestamp']
        })
    new_list_sorted: list[dict[str, str]] = sorted(new_list, reverse=True,
        key=lambda x: datetime.strptime(x['eventDate'], '%d.%m.%Y'))
    return new_list_sorted


def _download_img(new_data: list[dict[str, str]],
                  out_dir: str  # | os.PathLike
                  ) -> list[str]:
    out_filenames: list[str] = []
    for record in new_data:
        file_id = record['Image'].rsplit('id=')[-1]
        download_url = f'https://drive.google.com/uc?id={file_id}'
        # Send the request
        response = requests.get(download_url)
        ext: str = guess_extension(response.headers['Content-Type'])
        if response.status_code == 200 and ext:
            out_path: str = join(out_dir, f'{file_id}{ext}')
            out_filenames.append(f'{file_id}{ext}')
            if not isfile(out_path):
                print('Downloading ', out_path)
                with open(out_path, 'wb') as file:
                    file.write(response.content)
    return out_filenames


def _write_to_json(data: list[dict[str, str]],
                   out_file: str,  # | os.PathLike,
                   img_filenames: list[str]) -> None:
    new_data: list[dict[str, str]] = _parse_new(data, img_filenames)

    # Using json.JSONEncoder for customization
    class CustomEncoder(json.JSONEncoder):
        def default(self, obj):
            if isinstance(obj, tuple):
                return list(obj)
            return super().default(obj)

    json_result_custom = json.dumps(new_data, cls=CustomEncoder, indent=2)
    with open(out_file, 'w', encoding='utf-8') as file:
        file.write(json_result_custom)


def update_news(creds_file: str,  # | os.PathLike,
                sheet_id: str,
                img_dir: str,  # | os.PathLike,
                out_file: str  # | os.PathLike
                ) -> None:

    sheet = _authorize_get_sheet(creds_file, sheet_id)
    # Only continue if there were new responses since last update
    if _check_new_news(sheet, out_file):
        print(f'The gd sheet contains {len(sheet.col_values(1)) - 1} stories.')

        # Get news records from the sheet
        new_data: list[dict[str, str]] = _get_new_data(sheet)
        img_filenames: list[str] = _download_img(new_data, img_dir)
        _write_to_json(new_data, out_file, img_filenames)


if __name__ == "__main__":
    # local testing
    """
    creds_file: str = 'backend/clim4cast-website-news-f36ce6b09109.json'
    sheet_id: str = '1Nputh6hzR1YcLAcR8PLdebgJJKE4Yaa5UTu3RLMhlT4'
    img_dir: str = 'media/news/img/'
    out_file: str = 'media/news/news.json'
    """
    # server-side
    creds_file: str = '/srv/clim4cast/clim4cast-website-news-f36ce6b09109.json'
    sheet_id: str = '1Nputh6hzR1YcLAcR8PLdebgJJKE4Yaa5UTu3RLMhlT4'
    img_dir: str = '/var/www/clim4cast.czechglobe.cz/media/news/img/'
    out_file: str = '/var/www/clim4cast.czechglobe.cz/media/news/news.json'
    update_news(creds_file, sheet_id, img_dir, out_file)
