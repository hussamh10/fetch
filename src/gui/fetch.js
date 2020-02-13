module.exports = {
	init: init,
	show: show,
	hide: hide
}

const electron = require('electron');
const exec = require('child_process').exec;
const path = require('path');
const config = require('./config');

let queries = {};
let channel;
let tray;

function init() {
	// wait for renderer to establish communications
	electron.ipcMain.on('channel', (event, arg) => {
		channel = event;
		
		let cfg = config.get();

		// launch finder app
		let finder = exec([cfg.pythonPath, path.join(cfg.fetchPath, "main.py")].join(' '));

		// establish other links
		establishLinks(finder.stdin, finder.stdout);

		// configure indexer
		setInterval(runIndexer, cfg.indexerDuration * 60);

		// init tray
		initTray();

		// set theme
		setTheme(config.get().theme);
	});

}


// configure pipes for communication with renderer and finder processes
function establishLinks(stdin, stdout) {

	// search event
	electron.ipcMain.on('search', (event, arg) => {
		if (arg.trim() == '') {
			return;
		}
		// log the query
		queries[arg] = {'event': event};
		// send query to finder process
		stdin.write(arg + '\n');
		console.log(arg);
	});

	// target open event
	electron.ipcMain.on('open', (e, arg) => {
		if (arg) {
			console.log('opening', arg);
			electron.shell.openItem(arg);
		}
	});

	// target hide event
	electron.ipcMain.on('hide', (e, arg) => {
		electron.BrowserWindow.getFocusedWindow().hide();
	});

	// handle finder pipe
	stdout.on('data', (data) => {
		let lines = data.split(new RegExp('\r?\n'));
		let query = null, results = [];
		for (let line of lines) {
			if (line == ':indexed') {
				console.log('indexed');
				channel.reply('indexed');
			} else if (line.startsWith(':')) {
				query = line.slice(1, line.length);
				results = [];
			} else if (line != '') {
				results.push(line);
			}
		}
		// find relevant event and send results to view
		if (query != null) {
			let q = queries[query];
			if (q != null) {
				let event = q.event;
				results = results.map(x => x.split('|'));
				event.reply('results', results);
				delete queries[query];		
			}
		}
	});
}

// rebuild the finder indexer
function runIndexer() {
	console.log('rebuilding index');
	let cfg = config.get();
	exec([cfg.pythonPath, path.join(cfg.fetchPath, "index.py")].join(' '));
}

function show() {
	if (channel) {
		channel.reply('show');
	}
}

function hide() {
	if (channel) {
		channel.reply('hide');
	}
}

function initTray() {
	tray = new electron.Tray('app/res/fetch.png');
	const trayTemplate = [{
			label: 'Update index',
			click: runIndexer
		}, {
			label: 'Inject CSS',
			click: () => {
				setTheme('blue');
			}
		}, {
			label: 'Exit',
			click: electron.app.quit
		}
	];

	tray.setContextMenu(electron.Menu.buildFromTemplate(trayTemplate));
}

function setTheme(theme) {
	let path = getTheme(theme);
	channel.reply('set-theme', path);
}

function getTheme(themeName) {
	let themesList = getThemesList();

	// return requested theme
	if (themesList[themeName]) {
		return themesList[themeName];
	}
	
	// return default theme
	return themesList['default-light'];
}

function getThemesList() {
	// load external themes
	let themesList = config.getExternalThemesList();

	// insert default themes
	themesList['default-light'] = `file://${__dirname}/misc/default-light.css`;
	themesList['default-dark'] = `file://${__dirname}/misc/default-dark.css`;

	return themesList;
}
