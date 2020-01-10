exports.MainController = function ($rootScope, $scope, $location, $mdDialog) {

	$scope.$on('$routeChangeSuccess', function (event, current) {
		var path = current.$$route.originalPath.substr(1);
		// path needs to match the [name] property on the [md-nav-item] in index.html
		if (path === '') {
			path = 'home';
		}
		console.log('Navigated to', path);
		// $scope.$apply(function () {
		$scope.currentTab = path;
		// });
	});

	// var user = UserService.getUser();

	// login button
	this.toggleLogin = function (ev) {
		if (!$rootScope.isLoggedIn) {
			$mdDialog.show({
				controller: LoginController,
				templateUrl: 'views/login.html',
				parent: angular.element(document.body),
				targetEvent: ev,
				clickOutsideToClose: true,
			}).then(function (answer) {
				// ok button was clicked
				console.log('got answer from popup', answer);
				$rootScope.isLoggedIn = true;
			}, function () {
				// cancel button was clicked
			});
		} else {
			$rootScope.isLoggedIn = false;
		}
		console.log('toggling login to {' + $rootScope.isLoggedIn + '}');
	};
};

function LoginController($mdDialog, $scope) {
	$scope.login = function () {
		$mdDialog.hide({
			foo: 'bar',
			name: 'Jason is cool',
			speed: 'over 7000'
		});
	};
	$scope.cancel = function () {
		$mdDialog.cancel();
	};
}
