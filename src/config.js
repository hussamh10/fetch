const fs = require('fs').promises;
const path = require('path');
const { app } = require('electron');

module.exports = {
	get: get,
	put: put,
	getThemesList: getThemesList,
	getDefaultConfig: getDefaultConfig,
	makePath: makePath
}

function getConfigPath() {
	let dir;

	if (process.env.HOME) {
		// ubuntu machine
		dir = path.join(process.env.HOME, '.fetch');
	} else if (process.env.LOCALAPPDATA) {
		// windows machine
		dir = path.join(process.env.LOCALAPPDATA, 'Fetch');
	}
	return { dir: dir, file: 'config.json' };
}

async function getThemesList() {
	let configPath = getConfigPath();
	let themePath = path.join(configPath.dir, 'themes');

	let themeList = {};
	themeList['default-light'] = `file://${makePath('assets/themes/default-light.css')}`;
	themeList['default-dark'] = `file://${makePath('assets/themes/default-dark.css')}`;

	
	try {
		for (let i of await fs.readdir(themePath)) {
			let themeName = i.replace('.css', '');
			themeList[themeName] = `file://${path.join(themePath, i)}`;
		}	
	} catch (ignored) { /* custom themes dir doesn't exist */ }
	
	return themeList;
}

async function getDefaultConfig() {
	return JSON.parse(await fs.readFile(makePath('assets/config/default_config.json')));
}

async function get() {
	let configPath = getConfigPath();
	let configFile = path.join(configPath.dir, configPath.file);
	let config;

	try {
		// try loading the saved config
		config = JSON.parse(await fs.readFile(configFile));		
	} catch (e) {
		// saved config not found, rewrite default config
		let default_config = await getDefaultConfig();
		config = {};

		for (let key in default_config) {
			config[key] = default_config[key].default;
		}
		save(config);
	}
	
	return config;
}

function put(kv) {
	let config = get();
	for (let key in kv) {
		config[key] = kv[key];
	}
	save(config);
}

async function save(config) {
	let configPath = getConfigPath();
	try {
		await fs.mkdir(configPath.dir, { recursive: true });
	} catch (e) {
		// ignore folder exists error
		if (e.code != 'EEXIST') {
			throw e;
		}
	}
	let configFile = path.join(configPath.dir, configPath.file);
	await fs.writeFile(configFile, JSON.stringify(config));
}

function makePath(sub) {
	return path.join(app.getAppPath(), sub);
}