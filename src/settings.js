module.exports = { show: show, init: init };

const electron = require('electron');
const config = require('./config');
const { relaunch } = require('./fetch');
const AutoLaunch = require('auto-launch');

const autoLauncher = new AutoLaunch({
	name: 'Fetch',
	isHidden: true
});

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
			// devTools: false
		},
		frame: false,
		transparent: true
	});

	settingsWindow.setMenuBarVisibility(false);
	settingsWindow.loadFile(config.makePath('app/settingsApp/index.html'));
	settingsWindow.setIcon(config.makePath('assets/icons/icon.png'));
	settingsWindow.on('closed', () => {
		settingsWindow = null;
	});
}

function init() {
	electron.ipcMain.on('send-config', async(event, args) => {
		event.reply('config', await marshalConfig());
	});

	electron.ipcMain.on('save', async(event, args) => {
		let cfg = unmarshalConfig(args);
		await config.put(cfg);
		await applyConfig(cfg);
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

async function marshalConfig() {
	let cfg = await config.get();
	let configInfo = await config.getDefaultConfig();
	configInfo.theme.options = Object.keys(await config.getThemesList());
	
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

async function applyConfig(cfg) {
	if (cfg.launchOnStartup) {
		autoLauncher.enable();
	} else {
		autoLauncher.disable();
	}
}