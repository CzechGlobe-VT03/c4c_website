const options = {
    key: 'opW94sVl7ctkZ7UWHHYmwjj41FBXk5Pp',
    verbose: false,

    lat: 46.0,
    lon: 17.6,
    zoom: 5,
    favOverlays: ['fires', 'awr_0_40', 'awr_0_100', 'awp_0_40',
        'awp_0_100', 'awd_0_40', 'awd_0_100'], // fuel moisture is missing
    overlay: 'awp_0_40',
    expertMode: true
};

// Initialize Windy API
windyInit(options, windyAPI => {
    // windyAPI init
    const { store } = windyAPI;
    
    // Uncomment this to get all available layers
    console.log(store.getAllowed('product'));
    console.log(store.getAllowed('overlay'));
    // console.log(store.get('availLevels'));

    // adds CzechGlobe logo + link
    const czechGlobeA = document.createElement("a");
    czechGlobeA.href = 'https://www.czechglobe.cz/';
    czechGlobeA.target = '_top';
    const czechGlobeImg = document.createElement("img");
    czechGlobeImg.src = 'https://www.windy.com/img/providers/czechglobe.svg';
    czechGlobeImg.style = 'position: absolute; pointer-events: auto; top: 5px; left: 50%; width: 89px; margin-top: 1em; margin-left: 75px; filter:drop-shadow(0 0 2px rgba(0,0,0,0.8)); z-index: 400;';
    czechGlobeA.appendChild(czechGlobeImg);
    const logoWrapper = document.getElementById("logo-wrapper");
    if (logoWrapper) {
        logoWrapper.appendChild(czechGlobeA);
    }
});