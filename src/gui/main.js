const electron = require('electron');
const fetch = require('./fetch');
const config = require('./config');

// construct electron app
electron.app.whenReady().then(init);

function init() {
	setTimeout(() => {
		fetch.init();
		initWindow();
	}, 2000);
}

function initWindow() {
	let window = new electron.BrowserWindow({
		width: 400,
		height: 500,
		webPreferences: {
			nodeIntegration: true
		},
		// frame: false,
		// transparent: true
	});
	
	window.setMenuBarVisibility(false);
	window.loadFile('app/index.html');

	configureEvents(window);
	hideWindow(window);
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