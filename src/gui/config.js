const fs = require('fs');
const path = require('path');

module.exports = {
	get: get,
	put: put,
	getThemeList: getThemeList
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

function getThemeList() {
	let configPath = getConfigPath();
	let themePath = path.join(configPath.dir, 'themes');
	if (!fs.existsSync(themePath)) {
		return {};
	}
	let themeList = {};
	for (let i of fs.readdirSync(themePath)) {
		themeList[i] = path.join('file://', themePath, i);
	}
	return themeList;
}

function get() {
	let configPath = getConfigPath();
	let configFile = path.join(configPath.dir, configPath.file);
	if (!fs.existsSync(configFile)) {
		save(JSON.parse(fs.readFileSync('misc/default_config.json')));	
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