async function loadTranslations() {
    const response = await fetch('../lang.json');
    translations = await response.json();
    
    let savedLang = localStorage.getItem('lang');
    let browserLang = navigator.language.slice(0, 2); // Get browser language (e.g., 'en', 'fr')
    
    let lang = savedLang || (translations[browserLang] ? browserLang : 'en'); // Default to English if not available
    
    setLanguage(lang);
}

function setLanguage(lang) {
  localStorage.setItem('lang', lang);
  document.documentElement.lang = lang;
  document.getElementById("language-switcher").value = lang;
  // Navbar
  document.getElementById("nav-home").innerText = translations[lang]["home"];
  document.getElementById("nav-news").innerText = translations[lang]["news"];
  document.getElementById("nav-drought").innerText = translations[lang]["drought"];
  document.getElementById("nav-heatwave").innerText = translations[lang]["heatwave"];
  document.getElementById("nav-wildfire").innerText = translations[lang]["wildfire"];
  document.getElementById("nav-partners").innerText = translations[lang]["partners"];
  // Headings
  document.getElementById("h-windy").innerText = translations[lang]["windy_map"];
  document.getElementById("h-news").innerText = translations[lang]["news"];
  document.getElementById("h-drought").innerText = translations[lang]["drought"];
  document.getElementById("h-heatwave").innerText = translations[lang]["heatwave"];
  document.getElementById("h-wildfire").innerText = translations[lang]["wildfire"];
  document.getElementById("h-partners").innerText = translations[lang]["partners"];
  // Footer
  document.getElementById("copyright").innerText = translations[lang]["czechglobe"];
  document.getElementById("project-website").innerText = translations[lang]["project_web"];
  document.getElementById("contact").innerText = translations[lang]["contact"];
  // Drought
  document.getElementById("partner_products_drought").innerText = translations[lang]["partner_products_drought"];
  document.getElementById("btn-drought_intensity").innerText = translations[lang]["drought_intensity"];
  document.getElementById("btn-soil_moisture").innerText = translations[lang]["soil_moisture"];
  document.getElementById("btn-soil_saturation").innerText = translations[lang]["soil_saturation"];
  document.getElementById("most_recent_drought").innerText = translations[lang]["most_recent"];
  document.getElementById("last_week_drought").innerText = translations[lang]["last_week"];
  // Heatwave
  document.getElementById("partner_products_heatwave").innerText = translations[lang]["partner_products_heatwave"];
  document.getElementById("btn-heat_index").innerText = translations[lang]["heat_index"];
  document.getElementById("btn-utci").innerText = translations[lang]["utci"];
  document.getElementById("most_recent_heatwave").innerText = translations[lang]["most_recent"];
  document.getElementById("last_week_heatwave").innerText = translations[lang]["last_week"];
  // Wildfire
  document.getElementById("partner_products_wildfire").innerText = translations[lang]["partner_products_wildfire"];
  document.getElementById("btn-fire_spread").innerText = translations[lang]["fire_spread"];
  document.getElementById("btn-fuel_moisture").innerText = translations[lang]["fuel_moisture"];
  document.getElementById("most_recent_wildfire").innerText = translations[lang]["most_recent"];
  document.getElementById("last_week_wildfire").innerText = translations[lang]["last_week"];
  // Other
  document.getElementById("more_news").innerText = translations[lang]["more_news"];
  document.getElementById("supported_by").innerText = translations[lang]["supported_by"];

  // Map Style Toggle
  document.getElementById("toggle-label").innerText = `${translations[lang]["toggle_label"]}:`

  // Descriptions
  document.getElementById("drought_desc").innerHTML = getTranslatedDesc(document.getElementById("drought_fig").name);
  document.getElementById("heatwave_desc").innerHTML = getTranslatedDesc(document.getElementById("heatwave_fig").name);
  document.getElementById("wildfire_desc").innerHTML = getTranslatedDesc(document.getElementById("wildfire_fig").name);
}

function getTranslatedDesc(desc_name) {
  var currentLang = document.documentElement.lang
  return `<b>${translations[currentLang]["description"]}:</b> ${translations[currentLang][desc_name]}`
}

document.getElementById("language-switcher").addEventListener("change", (event) => {
  setLanguage(event.target.value);
});



/* End of temporary translation functionality - needs to be reworked */

/*
News section functionality - display the 8 latest news events
*/

async function getNews() {
  // Loads the news items from json
  const news = await fetch(`./media/news/news.json?t=${new Date().getTime()}`);
  const data = await news.json()
  return data
}

function defineNewsItem(news, news_section) {
  // Define html for a news event

  // Create the news item
  var news_item = document.createElement('button')
  news_item.className = 'news_item'
  news_item.title = news.eventDate + ', ' + news.countries
  var img_wrapper = document.createElement('div')
  var img_main = document.createElement('img')
  img_main.className = 'news_img'
  img_main.src = 'media/news/img/' + news.imgFilename
  var news_title = document.createElement('h4')
  news_title.innerText = news.eventTitle
  img_wrapper.appendChild(img_main)
  news_item.appendChild(img_wrapper)
  news_item.appendChild(news_title)

  // Create elements inside the modal
  var news_modal = document.createElement('div')
  news_modal.className = 'modal'

  var news_modal_content = document.createElement('div')
  news_modal_content.className = 'modal_content'
  var modal_close = document.createElement('span')
  modal_close.className = 'close'
  modal_close.innerText = '×'
  var modal_img_wrapper = document.createElement('div')
  modal_img_wrapper.className = 'modal_img_wrapper'
  var modal_img = document.createElement('img')
  modal_img.className = 'modal_news_img'
  modal_img.src = 'media/news/img/' + news.imgFilename
  var modal_title = document.createElement('h4')
  modal_title.innerText = news.eventTitle
  var modal_meta = document.createElement('p')
  modal_meta.innerText = news.eventDate + ', ' + news.countries + ' (' + news.eventTypes + ')'
  var modal_text = document.createElement('p')
  modal_text.className = 'modal_text'
  modal_text.innerText = news.eventDesc

  // Add all the created elements to the html
  news_modal_content.appendChild(modal_close)
  modal_img_wrapper.appendChild(modal_img)
  news_modal_content.appendChild(modal_img_wrapper)
  news_modal_content.appendChild(modal_title)
  news_modal_content.appendChild(modal_meta)
  news_modal_content.appendChild(modal_text)
  
  if (news.eventLink) {
    var modal_link = document.createElement('p')
    modal_link.className = 'modal_text'
    modal_link.innerHTML = 'More information:<br><a href="' + news.eventLink + '">' + news.eventLink + '</a>'
    news_modal_content.appendChild(modal_link)
  }

  news_modal.appendChild(news_modal_content)

  news_section.appendChild(news_item)
  news_section.appendChild(news_modal)
};

function defineMoreNews(news) {
  // Define the modal containing more news

  let more_news = document.getElementById('more_news').parentElement;

  var modal = document.createElement('div')
  modal.className = 'modal'

  var modal_content = document.createElement('div')
  modal_content.className = 'modal_content'

  var modal_close = document.createElement('span')
  modal_close.className = 'close'
  modal_close.innerText = '×'
  
  var modal_items_wrapper = document.createElement('div')
  modal_items_wrapper.className = 'more_items_wrapper'
  // Add individual stories to the modal
  for (let i = 0; i < news.length; i++) {
    var item = document.createElement('div')
    item.className = 'more_news_item'

    var modal_img = document.createElement('img')
    modal_img.className = 'modal_news_img'
    modal_img.src = 'media/news/img/' + news[i].imgFilename
    var modal_title = document.createElement('h4')
    modal_title.innerText = news[i].eventTitle
    var modal_meta = document.createElement('p')
    modal_meta.innerText = news[i].eventDate + ', ' + news[i].countries + ' (' + news[i].eventTypes + ')'
    var modal_text = document.createElement('p')
    modal_text.className = 'modal_text'
    modal_text.innerText = news[i].eventDesc
    var horizontal_line = document.createElement('hr')
    
    item.appendChild(modal_img)
    item.appendChild(modal_title)
    item.appendChild(modal_meta)
    item.appendChild(modal_text)
    if (news[i].eventLink) {
      var modal_link = document.createElement('p')
      modal_link.className = 'modal_text'
      modal_link.innerHTML = 'More information:<br><a href="' + news[i].eventLink + '">' + news[i].eventLink + '</a>'
      item.appendChild(modal_link)
    }
    item.appendChild(horizontal_line)

    modal_items_wrapper.appendChild(item)
  };

  modal_content.appendChild(modal_close)
  modal_content.appendChild(modal_items_wrapper)
  modal.appendChild(modal_content)
  more_news.append(modal)
};

function openCloseModal() {
  // Get the modal
  var modal = document.getElementsByClassName("modal");
  var last_index = modal.length - 1;
  // Get the button that opens the modal
  var btn = document.getElementsByClassName("news_item");
  // Get the <span> element that closes the modal
  var span = document.getElementsByClassName("close");

  for (let i = 0; i < btn.length; i++) {
    // When the user clicks on the button, open the modal
    btn[i].onclick = function() {
      modal[i].style.display = "block";
    }

    // When the user clicks on <span> (x), close the modal
    span[i].onclick = function() {
      modal[i].style.display = "none";
    }
  };

  // Open/close the more news modal
  var more_news = document.getElementById('more_news')
  more_news.onclick = function() {
    modal[last_index].style.display = "block";
  }
  span[last_index].onclick = function() {
    modal[last_index].style.display = "none";
  }

  // When the user clicks anywhere outside of the modal, close it
  window.onclick = function(event) {
    for (let i = 0; i < modal.length; i++) {
    if (event.target == modal[i]) {
      modal[i].style.display = "none";
    }
    }
  }
}

async function showNews() {
  // Defines the whole news section, loads news items and creates corresponding HTML

  let news_section = document.getElementById('news_section')
  var news = await getNews()

  for (let i = 0; i < Math.min(8, news.length); i++) {
    defineNewsItem(news[i], news_section)
  }
  defineMoreNews(news)

  openCloseModal()
};
showNews()

/*
Button functionality for displaying model results.
*/
function getFigData(value) {
  switch (value) {
    case 'AWR_40':
      return {
        value: value,
        filename: "AWR_0-40cm",
        type: "drought",
        desc_name: "soil_saturation_desc",
      };
    case 'AWR_100':
      return {
        value: value,
        filename: "AWR_0-100cm",
        type: "drought",
        desc_name: "soil_saturation_desc",
      };
    case 'AWR_200':
      return {
        value: value,
        filename: "AWR_0-200cm",
        type: "drought",
        desc_name: "soil_saturation_desc",
      };
    case 'AWP_40':
      return {
        value: value,
        filename: "AWP_0-40cm",
        type: "drought",
        desc_name: "drought_intensity_desc",
      };
    case 'AWP_100':
      return {
        value: value,
        filename: "AWP_0-100cm",
        type: "drought",
        desc_name: "drought_intensity_desc",
      };
    case 'AWP_200':
      return {
        value: value,
        filename: "AWP_0-200cm",
        type: "drought",
        desc_name: "drought_intensity_desc",
      };
    case 'AWD_40':
      return {
        value: value,
        filename: "AWD_0-40cm",
        type: "drought",
        desc_name: "soil_moisture_desc",
      };
    case 'AWD_100':
      return {
        value: value,
        filename: "AWD_0-100cm",
        type: "drought",
        desc_name: "soil_moisture_desc",
      };
    case 'AWD_200':
      return {
        value: value,
        filename: "AWD_0-200cm",
        type: "drought",
        desc_name: "soil_moisture_desc",
      };
    case 'HI':
      return {
        value: value,
        filename: "HI_cm",
        type: "heatwave",
        desc_name: "heat_index_desc",
      }
    case 'UTCI':
      return {
        value: value,
        filename: "UTCI_cm",
        type: "heatwave",
        desc_name: "utci_desc",
      }
    case 'FWI':
      return {
        value: value,
        filename: "FWI_GenZ_cm",
        type: "wildfire",
        desc_name: "fire_spread_desc",
      }
    case 'DFM10h':
      return {
        value: value,
        filename: "DFM10H_cm",
        type: "wildfire",
        desc_name: "fuel_moisture_desc",
      }
  }
}

function getFileSrc(filename) {
  var toggleSwitch = document.getElementById("toggleSwitch")
  if (toggleSwitch.checked) {
    return `media/figs_muni/${filename}.jpg?t=${new Date().getTime()}`
  } else {
    return `media/figs/${filename}.jpg?t=${new Date().getTime()}`
    //return `media/figs_windy/${filename}.jpg?t=${new Date().getTime()}`
  }
}

function setFigData(fig_data) {
  const fig = document.getElementById(fig_data.type + "_fig")
  const desc = document.getElementById(fig_data.type + "_desc")
  const download = document.getElementById(fig_data.type + "_download")
  const download_old = document.getElementById(fig_data.type + "_download_old")
  fig.src = getFileSrc(fig_data.filename)
  fig.name = fig_data.desc_name
  fig.dataset.datatype = fig_data.value
  //"media/figs/" + fig_data.filename + ".jpg"
  desc.innerHTML = getTranslatedDesc(fig_data.desc_name)
  download.href = getFileSrc(fig_data.filename)
  download_old.href = `media/figs_old/${fig_data.filename}.zip?t=${new Date().getTime()}`
  //"media/figs_old/" + fig_data.filename + ".zip"
}

function getPartnerLinks(fig_type) {
  switch (fig_type) {
    case "drought":
      return [
        {icon: "intersucho.png", url: "https://www.intersucho.cz/", title: "Intersucho"},
        {icon: "TU_Wien_GEO_Logo.png", url: "https://dataviewer.geo.tuwien.ac.at/", title: "TU Wien DataViewer"},
        {icon: "geosphere_logo.svg", url: "https://www.zamg.ac.at/incaanalyse", title: "GeoSphere Austria"},
        {icon: "IUNG_EN_transparent.png", url: "https://susza.iung.pulawy.pl/en/index/", title: "ADMS - Agricultural Drought Monitoring System"},
        {icon: "DHMZ logo.png", url: "https://meteo.hr/klima_e.php?section=klima_pracenje&param=spi", title: "Croatian cumulative precipitation"},
        {icon: "logo SHMU 600.png", url: "https://www.shmu.sk/sk/?page=1&id=monitoring_sucha", title: "Slovak drought monitoring"},
        {icon: "ARSO_meteo.png", url: "https://www.meteo.si/uploads/probase/www/agromet/bulletin/drought/sl/", title: "Slovenian drought monitoring"},
        ]
    case "heatwave":
      return [
        {icon: "geosphere_logo.svg", url: "https://warnungen.zamg.at/wsapp/de/alle/heute/-114294,119674,916956,672359", title: "GeoSphere Austria – warnings"},
        {icon: "DHMZ logo.png", url: "https://meteo.hr/prognoze_e.php?section=prognoze_specp&param=toplinskival_5", title: "Croatian heatwave warning system"},
        {icon: "ARSO_meteo.png", url: "https://meteo.arso.gov.si/met/sl/climate/current/heat_indices/", title: "Slovenian heatwave monitoring"},
        ]
    case "wildfire":
      return [
        {icon: "logo-firerisk-opt.svg", url: "https://www.firerisk.cz/", title: "FireRisk – Czechia"},
        {icon: "geosphere_logo.svg", url: "https://www.zamg.ac.at/cms/de/wetter/wetter-oesterreich/waldbrand", title: "Austrian forest fire warning system"},
        {icon: "logo SHMU 600.png", url: "https://www.shmu.sk/sk/?page=1&id=klimat_dennemapy", title: "Slovak fire risk monitoring"},
        {icon: "ARSO_meteo.png", url: "https://meteo.arso.gov.si/met/sl/warning/fire/", title: "Slovenian fire risk monitoring"},
        ]
  }
}

function setPartnerLinks(fig_type, partners) {
  const partner_div = document.getElementById(fig_type + "_partners")

  inner_html = ""
  for (var i = 0; i < partners.length; i++) {
    inner_html = inner_html + '<a href="' + partners[i].url + '" title="' +
      partners[i].title + '" target="_blank" rel="noopener noreferrer"><img src="media/logo/' +
      partners[i].icon + '" style="width:100%" /></a>'
  }
  partner_div.innerHTML = inner_html
}

async function changeBackgroundColor(target) {
  await document.querySelectorAll((`button[name="${target.name}"]`)).forEach(button => {
    button.style.backgroundColor = '';
  });
  target.style.backgroundColor = "#6c8793"
}

function showCorrectFig(event) {
  let fig_data = getFigData(event.target.value)
  setFigData(fig_data)

  changeBackgroundColor(event.target)

  let partner_links = getPartnerLinks(fig_data.type)
  setPartnerLinks(fig_data.type, partner_links)
}

function addFigEventListeners(element_name) {
  var radios = document.getElementsByName(element_name);

  showCorrectFig({target: radios[0]})

  for (var i = 0; i < radios.length; i++) {
      radios[i].addEventListener('click', showCorrectFig);
  }
}


window.onload = async function() {
  let translations = {};
  await loadTranslations();

  await addFigEventListeners("drought_btns");
  await addFigEventListeners("heatwave_btns");
  await addFigEventListeners("wildfire_btns");
  initToggle();
}


/**
 * Create a map of partners
 * returns version of IE or false, if browser is not Internet Explorer
 */

function highlightFeature(e) {
  var layer = e.target;
  layer.setStyle({
      fillColor: '#4e5056',
  });
  layer.bringToFront();
};

function resetFeature(e) {
  var layer = e.target;
  layer.setStyle({
    fillColor: '#5a8691',
  });
  layer.bringToFront();
};

function onClick(e) {
  e.target.openPopup()
}

function createPopup(feature) {
  let start_html = '<div class=popupContent>' + feature.properties.NAME + ':<br>'
  let org1_html = '<a href="'+feature.properties.org1_link+'" title="'+feature.properties.org1_title+'" target="_blank" rel="noopener noreferrer"><img src="' + feature.properties.org1_logo + '" width="100%" /></a>'
  if (feature.properties.hasOwnProperty('org2_link')) {
    let org2_html = '<br><a href="'+feature.properties.org2_link+'" title="'+feature.properties.org2_title+'" target="_blank" rel="noopener noreferrer"><img src="'+feature.properties.org2_logo+'" width="100%" /></a>'
    return start_html + org1_html + org2_html + '</div>'
  } else {
    return start_html + org1_html + '</div>'
  }
}

function onEachFeature(feature, layer) {
  layer.bindPopup(createPopup(feature), {autoPan:false})
  layer.on({
    mouseover: highlightFeature,
    mouseout: resetFeature,
    click: onClick,
  })
}

async function fetchShowMapData(map) {
  const response_land = await fetch('./media/map/land_earth.geojson');
  const geojson_land = await response_land.json();
  const response_countries = await fetch('./media/map/countries.geojson');
  const geojson_countries = await response_countries.json();

  await L.geoJson(geojson_land, {
    interactive: false,
    stroke: false,
    fillColor: '#d6e8e5',
    fillOpacity: 1,
  }).addTo(map);
  
  await L.geoJson(geojson_countries, {
    onEachFeature: onEachFeature,
    weight: 1,
    color: '#fafafa',
    opacity: 1,
    fillColor: '#5a8691',
    fillOpacity: 1,
    attribution: 'Made with <a href="https://www.naturalearthdata.com/">Natural Earth</a>'
  }).addTo(map); 
}


function map() {
  // Define map
  var map = L.map('map', {
    zoomControl: false,
    boxZoom: false,
    doubleClickZoom: false,
    dragging: false,
    scrollWheelZoom: false,
    keyboard: false,
  }).fitBounds([
    [60, 5],
    [42, 25]]);
  fetchShowMapData(map)
};
map();


/**
 * Switch for maps visualisations
 */
function initToggle() {
  const toggleSwitch = document.getElementById('toggleSwitch');
  const statusText = document.getElementById('status');
  
  toggleSwitch.addEventListener('change', () => {

    for (const key of ["drought_fig", "heatwave_fig", "wildfire_fig"]) {
      var fig = document.getElementById(key);
      console.log(fig)
      console.log(fig.dataset.datatype)
      setFigData(getFigData(fig.dataset.datatype))
    }

    if (toggleSwitch.checked) {
      statusText.textContent = 'MUNI';
    } else {
      statusText.textContent = 'Windy';
    }
  });
  
  toggleSwitch.dispatchEvent(new Event('change'));    
}


/**
 * detect IE
 * returns version of IE or false, if browser is not Internet Explorer
 */
function detectIE() {
  var ua = window.navigator.userAgent;

  var msie = ua.indexOf("MSIE ");
  if (msie > 0) {
    // IE 10 or older => return version number
    return parseInt(ua.substring(msie + 5, ua.indexOf(".", msie)), 10);
  }

  var trident = ua.indexOf("Trident/");
  if (trident > 0) {
    // IE 11 => return version number
    var rv = ua.indexOf("rv:");
    return parseInt(ua.substring(rv + 3, ua.indexOf(".", rv)), 10);
  }

  // var edge = ua.indexOf('Edge/');
  // if (edge > 0) {
  //     // Edge (IE 12+) => return version number
  //     return parseInt(ua.substring(edge + 5, ua.indexOf('.', edge)), 10);
  // }

  // other browser
  return false;
}

function setVideoHeight() {
  const videoRatio = 1920 / 1080;
  const minVideoWidth = 400 * videoRatio;
  let secWidth = 0,
    secHeight = 0;

  secWidth = videoSec.width();
  secHeight = videoSec.height();

  secHeight = secWidth / 2.13;

  if (secHeight > 600) {
    secHeight = 600;
  } else if (secHeight < 400) {
    secHeight = 400;
  }

  if (secWidth > minVideoWidth) {
    $("video").width(secWidth);
  } else {
    $("video").width(minVideoWidth);
  }

  videoSec.height(secHeight);
}

// Parallax function
// https://codepen.io/roborich/pen/wpAsm
var background_image_parallax = function ($object, multiplier) {
  multiplier = typeof multiplier !== "undefined" ? multiplier : 0.5;
  multiplier = 1 - multiplier;
  var $doc = $(document);
  $object.css({ "background-attatchment": "fixed" });
  $(window).scroll(function () {
    var from_top = $doc.scrollTop(),
      bg_css = "center " + multiplier * from_top + "px";
    $object.css({ "background-position": bg_css });
  });
};

$(window).scroll(function () {
  if ($(this).scrollTop() > 50) {
    $(".scrolltop:hidden").stop(true, true).fadeIn();
  } else {
    $(".scrolltop").stop(true, true).fadeOut();
  }

  // Make sticky header
  if ($(this).scrollTop() > 158) {
    $(".tm-nav-section").addClass("sticky");
  } else {
    $(".tm-nav-section").removeClass("sticky");
  }
});

let videoSec;

$(function () {
  if (detectIE()) {
    alert(
      "Please use the latest version of Edge, Chrome, or Firefox for best browsing experience."
    );
  }

  const mainNav = $("#tmMainNav");
  mainNav.singlePageNav({
    filter: ":not(.external)",
    offset: $(".tm-nav-section").outerHeight(),
    updateHash: true,
    beforeStart: function () {
      mainNav.removeClass("show");
    },
  });

  videoSec = $("#tmVideoSection");

  // Adjust height of video when window is resized
  $(window).resize(function () {
    setVideoHeight();
  });

  setVideoHeight();

  $(window).on("load scroll resize", function () {
    var scrolled = $(this).scrollTop();
    var vidHeight = $("#hero-vid").height();
    var offset = vidHeight * 0.6;
    var scrollSpeed = 0.25;
    var windowWidth = window.innerWidth;

    if (windowWidth < 768) {
      scrollSpeed = 0.1;
      offset = vidHeight * 0.5;
    }

    $("#hero-vid").css(
      "transform",
      "translate3d(-50%, " + -(offset + scrolled * scrollSpeed) + "px, 0)"
    ); // parallax (25% scroll rate)

    // Magnific pop up
    $(".tm-gallery").magnificPopup({
      delegate: "a", // child items selector, by clicking on it popup will open
      type: "image",
      gallery: { enabled: true },
      // other options
    });
  });

  // Parallax image background
  background_image_parallax($(".tm-parallax"), 0.4);
  background_image_parallax($(".tm-parallax-2"), 0.4);
  background_image_parallax($(".tm-parallax-3"), 0.4);
  background_image_parallax($(".tm-parallax-4"), 0.3);
  background_image_parallax($(".tm-parallax-5"), 0.4);
  background_image_parallax($(".tm-parallax-6"), 0.4);

  // Back to top
  $(".scroll").click(function () {
    $("html,body").animate({ scrollTop: $("#home").offset().top }, "1000");
    return false;
  });
});

// Open the Modal
function openModal() {
  document.getElementById("myModal").style.display = "block";
}

// Close the Modal
function closeModal() {
  document.getElementById("myModal").style.display = "none";
}