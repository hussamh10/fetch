module.exports = { show: show, init: init };

const electron = require('electron');
const config = require('./config');
const { relaunch } = require('./fetch');

let settingsWindow;

function show() {
	if (settingsWindow != null) {
		return;
	}

	settingsWindow = new electron.BrowserWindow({
		width: 500,
		height: 600,
		webPreferences: {
			nodeIntegration: true,
			devTools: false
		},
		frame: false,
		transparent: true
	});

	settingsWindow.setMenuBarVisibility(false);
	settingsWindow.loadFile(config.makePath('app/settingsApp/index.html'));
	settingsWindow.setIcon(config.makePath('app/res/fetch.png'));
	settingsWindow.on('closed', () => {
		settingsWindow = null;
	});
}

function init() {
	electron.ipcMain.on('send-config', (event, args) => {
		event.reply('config', marshalConfig());
	});

	electron.ipcMain.on('save', (event, args) => {
		config.put(unmarshalConfig(args));
		relaunch();
	});

	electron.ipcMain.on('cancel', (event, args) => {
		close();
	});
}

function close() {
	if (settingsWindow) {
		settingsWindow.close();
	}
}

function marshalConfig() {
	let cfg = config.get();
	let configInfo = config.getDefaultConfig();
	configInfo.theme.options = Object.keys(config.getThemesList());
	
	for (let key in cfg) {
		if (configInfo[key]) {
			configInfo[key].value = cfg[key];
		}
		if (configInfo[key].type == 'array') {
			configInfo[key].value = configInfo[key].value.join('\n');
		}
	}

	return configInfo;
}

function unmarshalConfig(configInfo) {
	let config = {};
	for (let key in configInfo) {
		let value = configInfo[key].value;
		if (value) {
			config[key] = value;
			if (configInfo[key].type == 'array') {
				config[key] = value.split(new RegExp('\r?\n'));
			}
		}
	}
	return config;
}