const fs = require('fs');
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

function getThemesList() {
	let configPath = getConfigPath();
	let themePath = path.join(configPath.dir, 'themes');

	let themeList = {};
	themeList['default-light'] = `file://${makePath('assets/themes/default-light.css')}`;
	themeList['default-dark'] = `file://${makePath('assets/themes/default-dark.css')}`;

	if (fs.existsSync(themePath)) {
		for (let i of fs.readdirSync(themePath)) {
			let themeName = i.replace('.css', '');
			themeList[themeName] = path.join('file://', themePath, i);
		}
	}
	
	return themeList;
}

function getDefaultConfig() {
	return JSON.parse(fs.readFileSync(makePath('assets/config/default_config.json')));
}

function get() {
	let configPath = getConfigPath();
	let configFile = path.join(configPath.dir, configPath.file);
	if (!fs.existsSync(configFile)) {
		let default_config = getDefaultConfig();
		let config = {};
		for (let key in default_config) {
			config[key] = default_config[key].default;
		}
		save(config);
	}
	return JSON.parse(fs.readFileSync(configFile));
}

function put(kv) {
	let config = get();
	for (let key in kv) {
		config[key] = kv[key];
	}
	save(config);
}

function save(config) {
	let configPath = getConfigPath();
	if (!fs.existsSync(configPath.dir)) {
		fs.mkdirSync(configPath.dir, { recursive: true });
	}
	let configFile = path.join(configPath.dir, configPath.file);
	fs.writeFileSync(configFile, JSON.stringify(config));
}

function makePath(sub) {
	return path.join(app.getAppPath(), sub);
}