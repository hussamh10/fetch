const fs = require('fs');
const path = require('path');

module.exports = getConfig();

function getConfig() {
	let configPath;
	if (process.env.HOME) {
		configPath = path.join(process.env.HOME, '.fetch', 'config.json');
	} else if (process.env.LOCALAPPDATA) {
		configPath = path.join(process.env.LOCALAPPDATA, 'Fetch', 'config.json');
	}

	// load default config
	let config = JSON.parse(fs.readFileSync('default_config.json'));
	

	// return saved config if exists
	if (fs.existsSync(configPath)) {
		let savedConfig = JSON.parse(fs.readFileSync(configPath));
	
		// replace missing config with defaults
		for (let key in savedConfig) {
			config[key] = savedConfig[key];
		}
	}

	return config;
}
