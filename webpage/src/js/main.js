firebase.initializeApp({
	apiKey: "AIzaSyAlYl1Irm_8PHXY82xvriTXTF1t9DJazEM",
	authDomain: "infodesk-73f2a.firebaseapp.com",
	databaseURL: "https://infodesk-73f2a.firebaseio.com",
	projectId: "infodesk-73f2a",
	storageBucket: "",
	messagingSenderId: "1029971047611"
});

import { FAQController } from './FAQController.js';
import { MainController } from './MainController.js';
import { HomeController } from './HomeController.js';
import { ChatController } from './ChatController.js';
import { TimeFromFilter } from './TimeFromFilter.js';

angular.module('byu-faq', ['ngRoute', 'ngMaterial', 'firebase', 'ngStorage'])
	.config(function ($routeProvider, $mdThemingProvider) {

		$mdThemingProvider.definePalette('dark-blue', {
			'50': 'e2eaf2',
			'100': 'b6cbdf',
			'200': '85a9ca',
			'300': '5487b4',
			'400': '306da4',
			'500': '0b5394',
			'600': '0a4c8c',
			'700': '084281',
			'800': '063977',
			'900': '032965',
			'A100': '95b6ff',
			'A200': '0b5394',
			'A400': '2f71ff',
			'A700': '155fff',
			'contrastDefaultColor': 'light',
			'contrastDarkColors': [
			  '50',
			  '100',
			  '200',
			  'A100',
			  'A200'
			],
			'contrastLightColors': [
			  '300',
			  '400',
			  '500',
			  '600',
			  '700',
			  '800',
			  '900',
			  'A400',
			  'A700'
			]
		  });

		$mdThemingProvider.theme('default')
		.primaryPalette('dark-blue')
		.accentPalette('dark-blue')
		.backgroundPalette('grey');

		 $mdThemingProvider.theme('other')
		 .primaryPalette('blue')
		 .accentPalette('blue')
		 .backgroundPalette('light-blue')
		 .dark();

		$routeProvider
			.when("/", {
				templateUrl: "views/home.html",
				controller: HomeController,
				controllerAs: 'ctrl'
			})
			.when("/faqs", {
				templateUrl: "views/faqs.html",
				controller: FAQController,
				controllerAs: 'ctrl'
			})
			.when("/chat", {
				templateUrl: "views/chat.html",
				controller: ChatController,
				controllerAs: 'ctrl'
			})
			.otherwise('/');
	})
	.controller('MainController', MainController)
	.filter('timefrom', TimeFromFilter)
	.factory('UserService', function ($sessionStorage) {

		function randomIndex() {
			return Math.floor(Math.random() * 5);
		}

		function getRandomName() {
			var names = ['Jason', 'Brian', 'Kathryn', 'Erin', 'John', 'Jessica', 'Kirsten'];
			return names[randomIndex(names.length)];
		}

		function getNewRandomUser() {
			return {
				id: (Date.now().toString(36) + Math.random().toString(36).substr(2, 5)).toUpperCase(),
				name: getRandomName()
			};
		}

		return {
			getUser: function () {
				var user = $sessionStorage.user;
				if (!user) {
					user = getNewRandomUser();
					$sessionStorage.user = user;
				}
				console.log('found user', user);
				return user;
			}
		};
	});
