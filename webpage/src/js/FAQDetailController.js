exports.FAQDetailController = function($scope, $mdDialog, $firebaseObject, questionId) {
  console.log(questionId);
  var questionRef = firebase.database().ref('faq/' + questionId);
  $scope.question = $firebaseObject(questionRef);

  $scope.cancel = function() {
    $mdDialog.cancel();
  };

  $scope.answer = function() {
    $scope.question.updated = firebase.database.ServerValue.TIMESTAMP;
    $scope.question.$save();
    $mdDialog.hide(true); // sends to promise in FAQController.js
  };
};
