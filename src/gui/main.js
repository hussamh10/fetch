const electron = require('electron');
const exec = require('child_process').exec;

let queries = {};

function initFetch() {
	// launch finder app
	let finder = exec('/usr/bin/python3.6 test.py');
	// establish link with view
	initIPCMain(finder.stdin, finder.stdout);
	// render view window
	initWindow();
}

function initWindow() {
	let window = new electron.BrowserWindow({
		width: 500,
		height: 500,
		// frame: false,
		// titleBarStyle: 'hidden',
		webPreferences: {
			nodeIntegration: true
		}
	});
	
	// window.setMenuBarVisibility(false);
	window.loadFile('index.html');
}

function initIPCMain(stdin, stdout) {

	electron.ipcMain.on('query', (event, arg) => {
		// log the query
		queries[arg] = {'event': event};
		// send query to finder process
		stdin.write(arg + '\n');
		console.log(arg);
	});

	stdout.on('data', (data) => {
		let lines = data.split(new RegExp('\r?\n'));
		let query = null, results = [];
		for (let line of lines) {
			// is this a new result set?
			console.log('line', line);
			if (line.startsWith(':')) {
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
			let event = queries[query].event;
			event.reply('results', results);
			delete queries[query];	
		}
	});
}

electron.app.whenReady().then(initFetch);