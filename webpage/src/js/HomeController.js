
exports.HomeController = function ($scope, $location, $firebaseArray) {
// 	var askedQs = firebase.database().ref('/questions/');
	var faq = firebase.database().ref('faq/');
	$scope.faqs = $firebaseArray(faq);
};
