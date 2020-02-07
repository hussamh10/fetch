module.exports = {
	init: init,
	show: show,
	hide: hide
}

const electron = require('electron');
const exec = require('child_process').exec;
const config = require('./config');

let queries = {};
let channel;

function init() {
	// launch finder app
	let finder = exec([config.pythonPath, config.fetchPath].join(' '));
	// establish link with view
	initIPCMain(finder.stdin, finder.stdout);
}

function initIPCMain(stdin, stdout) {

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
	electron.ipcMain.on('open', (event, arg) => {
		if (arg) {
			console.log('opening', arg);
			electron.shell.openItem(arg);
		}
	});

	// communication channel establish event
	electron.ipcMain.on('channel', (event, arg) => {
		channel = event;
	});

	stdout.on('data', (data) => {
		let lines = data.split(new RegExp('\r?\n'));
		let query = null, results = [];
		for (let line of lines) {
			// is this a new result set?
			console.log('line', line);
			if (line == ':indexed') {
				console.log('indexed');
			} else if (line.startsWith(':')) {
				console.log('here');
				query = line.slice(1, line.length);
				results = [];
			} else {
				results.push(line);
			}
		}
		console.log('results for', query, data);
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

function show() {
	channel.reply('show');
}

function hide() {
	channel.reply('hide');
}