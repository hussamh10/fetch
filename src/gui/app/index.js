const { ipcRenderer, remote } = require('electron');
const { Tray } = remote;

let app = angular.module('fetchApp', []);

app.controller('fetchController', fetchController);

fetchController.$inject = ['$timeout'];

function fetchController($timeout) {
	let vm = this;
	
	init();

	function init() {
		vm.indexing = true;
		vm.search = search;
		vm.open = open;
		vm.openTopResult = openTopResult;
		vm.buttonKeyup = buttonKeyup;

		// establish a communication channel from main to renderer
		ipcRenderer.send('channel');

		initIPC();
		initTray();
	}
	
	function initIPC() {
		// handle results receive
		ipcRenderer.on('results', (event, arg) => {
			digest(() => { vm.results = arg; });
		});

		// handle channel events

		// when window is shown
		ipcRenderer.on('show', (event, arg) => {
			document.getElementById('search-bar').focus();
		});

		// when window is hidden
		ipcRenderer.on('hide', (event, arg) => {
			digest(clear);
		});

		ipcRenderer.on('indexed', (event, arg) => {
			digest(() => { vm.indexing = false; })
		})
	}

	function initTray() {
		let tray = new Tray('app/res/fetch.png');
		tray.setTitle('Fetch');
		tray.setToolTip('Fetch');
	}

	function search(query) {
		vm.results = null;
		ipcRenderer.send('search', query);
	}
	
	function openTopResult() {
		if (vm.results && vm.results.length > 0) {
			open(vm.results[0][1]);
		}
	}

	function open(path) {
		clear();
		ipcRenderer.send('open', path);
	}

	function clear() {
		vm.results = null;
		vm.query = null;
	}

	function digest(f) {
		$timeout(f, 0);
	}	

	function buttonKeyup(ev) {
		// capture only required keys
		if (!['ArrowDown', 'ArrowUp'].includes(ev.key)) {
			return;
		}

		let id = ev.target.id;

		if (id == 'search-bar') {
			nextId = 'button0';
			prevId = 'button3';
		} else if (id.startsWith('button')) {
			let c = parseInt(id.replace('button', ''));
			nextId = (c + 1) == 4 ? 'search-bar' : `button${c + 1}`
			prevId = (c - 1) == -1 ? 'search-bar' : `button${c - 1}`
		}

		if (ev.keyCode == 40) {
			// down key
			document.getElementById(nextId).focus();
		} else if (ev.keyCode == 38) {
			// up key
			document.getElementById(prevId).focus();
		}
	}
	
}