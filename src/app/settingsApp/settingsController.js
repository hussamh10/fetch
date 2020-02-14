(function(){

	const { ipcRenderer } = require('electron');

	let app = angular.module('settingsApp', []);

	app.controller('settingsController', settingsController);

	settingsController.$inject = ['$timeout'];

	function settingsController($timeout) {
		let vm = this;
		
		init();

		function init() {
			vm.save = save;
			vm.cancel = cancel;
			vm.adjustHeight = adjustHeight;
			
			// request config from main
			ipcRenderer.send('send-config');

			// init communication channels
			initIPC();
		}
		
		function initIPC() {
			// handle config receive
			ipcRenderer.on('config', (event, arg) => {
				digest(() => { vm.config = arg; });
			});
		}

		function save(config) {
			ipcRenderer.send('save', config);
		}

		function cancel() {
			ipcRenderer.send('cancel');
		}

		function digest(f) {
			$timeout(f, 0);
		}

		function adjustHeight(ev) {
			let o = ev.target;
			o.style.height = "1px";
			o.style.height = (25+o.scrollHeight)+"px";  
		}
		
	}
})();
