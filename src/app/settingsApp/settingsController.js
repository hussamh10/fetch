(function(){

	const { ipcRenderer } = require('electron');
	const isAccelerator = require('electron-is-accelerator');

	let app = angular.module('settingsApp', []);

	app.controller('settingsController', settingsController);

	settingsController.$inject = ['$timeout'];

	function settingsController($timeout) {
		let vm = this;
		
		init();

		function init() {
			vm.save = save;
			vm.cancel = cancel;
			vm.resetHeight = resetHeight;
			vm.adjustHeight = adjustHeight;
			vm.allConfigValid = allConfigValid;
			
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

		function resetHeight(ev) {
			let o = ev.target;
			o.style.height = "40px";
		}

		function adjustHeight(ev) {
			let o = ev.target;
			o.style.height = "1px";
			o.style.height = (25+o.scrollHeight)+"px";  
		}

		function isValidConfig(key, value) {
			switch(key) {
				case 'launchKey':
					if (!isAccelerator(value)) {
						return false;
					}
				break;
			}
			return true;
		}

		function allConfigValid(config) {
			for (let key in config) {
				config[key].valid = true;
				if (!isValidConfig(key, config[key].value)) {
					config[key].valid = false;
					return false;
				}
			}
			return true;
		}
		
	}
})();
