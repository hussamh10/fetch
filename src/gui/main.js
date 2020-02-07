const electron = require('electron');
const fetch = require('./fetch');
const config = require('./config');

// construct electron app
electron.app.whenReady().then(init);

function init() {
	fetch.init();
	initWindow();
}

function initWindow() {
	let window = new electron.BrowserWindow({
		width: 500,
		height: 500,
		// frame: false,
		// titleBarStyle: 'hidden',
		webPreferences: {
			nodeIntegration: true
		}
	});
	
	// window.setMenuBarVisibility(false);
	window.loadFile('app/index.html');

	configureEvents(window);
}

function configureEvents(window) {
	window.on('blur', () => {
		hideWindow(window);
	})

	electron.globalShortcut.register(config.launchKey, () => {
		if (window.isVisible()) {
			hideWindow(window);
		} else {
			showWindow(window);
		}
	})
}

function showWindow(window) {
	window.show();
	fetch.show();
}

function hideWindow(window) {
	window.hide();
	fetch.hide();
}