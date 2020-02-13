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
		height: 200,
		webPreferences: {
			nodeIntegration: true
		},
		frame: false,
		transparent: true
	});
	
	window.setMenuBarVisibility(false);
	window.loadFile('app/index.html');

	configureEvents(window);
	hideWindow(window);
}

function configureEvents(window) {
	window.on('blur', () => {
		hideWindow(window);
	});

	let cfg = config.get();

	electron.globalShortcut.register(cfg.launchKey, () => {
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