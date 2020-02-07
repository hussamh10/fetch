const { ipcRenderer, remote } = require('electron');
const { Tray } = remote;

let app = angular.module('fetchApp', []);

app.controller('fetchController', fetchController);

fetchController.$inject = ['$timeout'];

function fetchController($timeout) {
	let vm = this;
	
	init();

	function init() {
		vm.search = search;
		vm.open = open;
		vm.openTopResult = openTopResult;

		initIPC();
		initTray();
	}
	
	function initIPC() {
		// handle results receive
		ipcRenderer.on('results', (event, args) => {
			digest(() => { vm.results = args; });
		});

		// handle channel events

		// when window is shown
		ipcRenderer.on('show', (event, args) => {
			document.getElementById('search-bar').focus();
		});

		// when window is hidden
		ipcRenderer.on('hide', (event, args) => {
			digest(clear);
		});

		// establish a communication channel from main to renderer
		ipcRenderer.send('channel');

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
	
}