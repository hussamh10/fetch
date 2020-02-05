const { ipcRenderer } = require('electron');

let app = angular.module('fetchApp', []);

app.controller('fetchController', fetchController);

fetchController.$inject = ['$timeout'];

function fetchController() {
	let vm = this;
	
	init();


	function init() {
		vm.search = search;

		ipcRenderer.on('results', (event, args) => {
			vm.logger = args;
		});
	}
	
	function search(query) {
		ipcRenderer.send('query', query);
	}
	
}