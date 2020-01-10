import { FAQDetailController } from './FAQDetailController.js';

exports.FAQController = function ($scope, $firebaseArray, $location, $mdDialog) {
  var faq = firebase.database().ref('faq/');
  $scope.faqs = $firebaseArray(faq);
  $scope.newFAQ = {
    question: '',
    answer: '',
    link: ''
  };

  this.onEdit = function (event, item) {

    var editPopOptions = {
      locals: { questionId: item.$id },
      controller: FAQDetailController,
      templateUrl: 'views/faq_detail.html',
      parent: angular.element(document.body),
      targetEvent: event,
      clickOutsideToClose: true,
      fullscreen: false // Only for -xs, -sm breakpoints.
    };

    $mdDialog.show(editPopOptions).then(function (result) {
      console.log("ANSWER: " + result);
      faq.answer = result;

    }, function () {
      console.log("Clicked Cancel");
    });
  };

  this.onDelete = function (event, item) {
    var delOrKeepPopOptions = $mdDialog.confirm()
      .title('Do you really want to delete this FAQ?')
      .textContent("Q: " + item.question) // I need to fix this.....
      .targetEvent(event)
      .ok("Delete")
      .cancel("Keep");

    $mdDialog.show(delOrKeepPopOptions).then(function (result) {
      console.log("ANSWER: " + result);
      $scope.faqs.$remove(item);
      //add question to answeredMsgs and delete from unanswered
    }, function () {
      console.log("Clicked Cancel");
    });

  };

  this.onSubmitNewFAQ = function () {
    if (!$scope.newFAQ.question || !$scope.newFAQ.answer) {
      console.log('question and answer are required for a new question');
      return;
    }

    $scope.faqs.$add({
      question: $scope.newFAQ.question,
      answer: $scope.newFAQ.answer,
      link: $scope.newFAQ.link,
      answered: true,
      onFAQ: true,
      created: firebase.database.ServerValue.TIMESTAMP,
      updated: firebase.database.ServerValue.TIMESTAMP
    });

    $scope.newFAQ = {
      question: '',
      answer: '',
      link: ''
    };

    $scope.faqAddForm.$setPristine();
    $scope.faqAddForm.$setUntouched();
  };
};
