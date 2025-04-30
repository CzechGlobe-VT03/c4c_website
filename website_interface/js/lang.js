let translations = {};

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

}

document.getElementById("language-switcher").addEventListener("change", (event) => {
    setLanguage(event.target.value);
});

loadTranslations();