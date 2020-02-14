const electron = require('electron');
const fetch = require('./fetch');
const settings = require('./settings');
const config = require('./config');

// construct electron app
electron.app.whenReady().then(init);

function init() {
	setTimeout(() => {
		settings.init();
		fetch.init();
		initWindow();
	}, 2000);
}

function initWindow() {
	let window = new electron.BrowserWindow({
		width: 400,
		height: 250,
		webPreferences: {
			nodeIntegration: true,
			devTools: false
		},
		frame: false,
		transparent: true,
		// resizable: false
	});
	
	window.setMenuBarVisibility(false);
	window.loadFile(config.makePath('app/fetchApp/index.html'));
	window.setIcon(config.makePath('app/res/fetch.png'));

	configureEvents(window);
	hideWindow(window);
}

function configureEvents(window) {
	window.on('blur', () => {
		hideWindow(window);
	});

	let launchKey = config.get().launchKey;
	electron.globalShortcut.register(launchKey, () => {
		if (window.isVisible()) {
			hideWindow(window);
		} else {
			adjustWindowPosition(window);
			showWindow(window);
		}
	});
}

function adjustWindowPosition(window) {
	const { width } = electron.screen.getPrimaryDisplay().workAreaSize;
	let x = (width / 2) - (400 / 2);
	let y = 200;
	window.setPosition(x, y);
}

function showWindow(window) {
	window.show();
	fetch.show();
}

function hideWindow(window) {
	window.hide();
	fetch.hide();
}
