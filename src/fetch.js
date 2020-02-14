module.exports = {
	init: init,
	show: show,
	hide: hide,
	relaunch: relaunch
}

const electron = require('electron');
const execFile = require('child_process').execFile;
const config = require('./config');
const settings = require('./settings');

let queries = {};
let channel;
let tray;

function init() {
	// wait for renderer to establish communications
	electron.ipcMain.on('channel', (event, arg) => {
		channel = event;
		
		let cfg = config.get();

		// launch finder app
		let finder = exec(config.makePath('main'));

		// establish other links
		establishLinks(finder.stdin, finder.stdout);

		// configure indexer
		setInterval(runIndexer, cfg.indexerDuration * 60 * 1000);

		// init tray
		initTray();

		// set theme
		setTheme(cfg.theme);

		// re-run indexer
		runIndexer();
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
	});

	// target open event
	electron.ipcMain.on('open', (e, arg) => {
		if (arg) {
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
	exec(config.makePath('index'));
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
	tray = new electron.Tray(config.makePath('app/res/fetch.png'));
	const trayTemplate = [{
			label: 'Update index',
			click: runIndexer
		}, {
			label: 'Settings',
			click: settings.show
		}, {
			label: 'Exit',
			click: electron.app.quit
		}
	];

	tray.setContextMenu(electron.Menu.buildFromTemplate(trayTemplate));
}

function setTheme(themeName) {
	let themes = config.getThemesList();
	
	// use default theme if invalid selected
	if (!themes[themeName]) {
		themeName = 'default-light';
	}

	channel.reply('set-theme', themes[themeName]);
}

function relaunch() {
	electron.app.relaunch();
	electron.app.quit();
}

function exec(exe) {
	if (process.platform.includes('win')) {
		exe = `${exe}.exe`;
	}
	return execFile(exe);
}