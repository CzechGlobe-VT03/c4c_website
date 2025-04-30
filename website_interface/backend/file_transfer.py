"""This script moves new data from the hydrospace.czechglobe server to this
web server. Created to be run once per day."""
import pysftp
import zipfile
from os import listdir, replace, remove, rename, stat
from os.path import join, exists
import os
import re
from datetime import datetime, date


def read_connection_info(file_path: str) -> dict[str, str]:
    login_info: dict[str, str] = {}
    try:
        with open(file_path, 'r') as file:
            for line in file:
                if line:  # Make sure the line is not empty
                    key, value = line.split(':')
                    login_info[key.strip()] = value.strip()
    except FileNotFoundError:
        print(f"The file {file_path} does not exist.")
    except Exception as e:
        print(f"An error occurred: {e}")
    return login_info


def repackage_old_figs(in_dir: str, out_dir: str, fnames: list[str]) -> None:
    """Moves old figures to the zip file"""
    old_figs: list[str] = listdir(new_dir)
    temp_zip_filename: str = join(out_dir, 'temp.zip')

    for fig, fname in zip(old_figs, fnames):
        # Remove oldest file from each archive
        name: str = fig.rsplit('.', 1)[0]
        zip_filename: str = join(out_dir, f'{name}.zip')
        if exists(zip_filename):
            with zipfile.ZipFile(zip_filename, 'r') as zipf:
                with zipfile.ZipFile(temp_zip_filename, 'w') as temp_zipf:
                    # Sort files in the original zip archive
                    sorted_infolist: list[zipfile.ZipInfo] = sorted(
                        zipf.infolist(), key=lambda x: x.filename)
                    # If there are already 7 days in the archive
                    if len(sorted_infolist) >= 7:
                        # Loop over all files in the zip except the oldest one
                        for item in sorted_infolist[1:]:
                            temp_zipf.writestr(item, zipf.read(item.filename))
            replace(temp_zip_filename, zip_filename)

        # Add new file to the archive
        fig_filename: str = join(in_dir, fig)
        with zipfile.ZipFile(zip_filename, 'a') as zipf:
            zipf.write(fig_filename, arcname=fname)


def add_new_figs(out_dir: str, c_info_path: str) -> str:
    # delete old files
    for file in listdir(out_dir):
        remove(join(out_dir, file))

    c_info: dict[str, str] = read_connection_info(c_info_path)
    cnopts = pysftp.CnOpts()
    cnopts.hostkeys = None
    with pysftp.Connection(c_info['host'],
                           username=c_info['username'],
                           password=c_info['password'],
                           cnopts=cnopts) as sftp:
        # Get dates curently on the server
        dates: list[str] = sftp.listdir('data')
        dates = [f for f in dates if re.fullmatch(r"\d{4}-\d{2}-\d{2}", f)]
        dates.sort()
        print(f'Downloading new images from date {dates[-1]}')
        # Copy figs from the latest date
        sftp.get_d(f'data/{dates[-1]}/JPG', out_dir)
    # remove date from the new filenames
    old_fnames: list[str] = listdir(out_dir)
    new_fnames: list[str] = [f'{fname.rsplit("_", 1)[0]}.jpg'
                             for fname in old_fnames]
    for old_file, new_file in zip(old_fnames, new_fnames):
        rename(join(out_dir, old_file), join(out_dir, new_file))
    # return filenames to be used in the archive
    return old_fnames


def update(new_dir: str, old_dir: str, connection_file: str) -> None:
    filenames = add_new_figs(new_dir, connection_file)
    repackage_old_figs(new_dir, old_dir, filenames)


def _check_new_data(new_dir: str, old_dir: str, connection_file: str) -> bool:
    """Are there new images to download? Checks both if there are new images
    and if they have already been downloaded"""

    today: date = date.today()
    print(f'Today: {today}')

    # Get modified date for the local files
    local_files: list[str] = listdir(new_dir)
    try:
        file_stats: dict[str, str] = stat(join(new_dir, local_files[0]))
        date_local: date = date.fromtimestamp(file_stats.st_mtime)
        print(f"Local latest: {date_local}")
    except IndexError:
        print(f'There are no images in the local folder {new_dir}.')
        date_local: date = date(1970, 1, 1)

    if today == date_local:
        print(f'New images have already been downloaded today [{today}].')
    else:
        c_info: dict[str, str] = read_connection_info(connection_file)
        cnopts = pysftp.CnOpts()
        cnopts.hostkeys = None
        with pysftp.Connection(c_info['host'],
                               username=c_info['username'],
                               password=c_info['password'],
                               cnopts=cnopts) as sftp:
            # Get dates curently on the server
            dates: list[str] = sftp.listdir('data')
            dates = [f for f in dates if re.fullmatch(r"\d{4}-\d{2}-\d{2}", f)]
            dates.sort()
            # list files in the latest folder - used for checking
            generated_imgs = sftp.listdir(join('data', dates[-1], 'JPG'))

        date_sftp = datetime.strptime(dates[-1], '%Y-%m-%d').date()

        if today != date_sftp:
            print("Todays figures are not available yet.")
        elif not generated_imgs:
            print("The SFTP contains a folder for today, but not the images.")
        else:
            print(f'Downloading new figures from today [{today}].')
            update(new_dir, old_dir, connection_file)


def on_fail() -> None:
    "Sends email if the process fails"
    raise NotImplementedError


if __name__ == '__main__':
    """
    # local testing
    new_dir: str = 'media/figs'
    old_dir: str = 'media/figs_old'
    connection_file: str = 'backend/connection.txt'
    """
    new_dir: str = '/var/www/clim4cast.czechglobe.cz/media/figs'
    old_dir: str = '/var/www/clim4cast.czechglobe.cz/media/figs_old'
    connection_file: str = '/srv/clim4cast/connection.txt'

    _check_new_data(new_dir, old_dir, connection_file)
    #update(new_dir, old_dir, connection_file)
