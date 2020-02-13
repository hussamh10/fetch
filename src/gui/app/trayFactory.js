(function(){
	const { remote, Menu } = require('electron');
	const { Tray } = remote;
	
	let app = angular.module('fetchApp');
	
	app.factory('trayFactory', trayFactoryWrapper);
	
	function trayFactoryWrapper() {
		
		function trayFactory() {
			let tray = new Tray('app/res/fetch.png');
			tray.setTitle('Fetch');
			tray.setToolTip('Fetch');

			this.tray = tray;
		}

		angular.extend(trayFactory.prototype, {
			buildMenu: function() {
				
			}
		});
	
		return trayFactory;
	}
})();
