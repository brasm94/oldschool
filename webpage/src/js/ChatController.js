exports.ChatController = function ($scope, $firebaseArray, $firebaseObject, $mdDialog, $timeout, $mdToast, UserService) {

  var user = UserService.getUser();
  var notiIsUp = false;

  var myMsgs = firebase.database().ref('/userMsgs/' + user.id);
  $scope.myQuestions = $firebaseArray(myMsgs);
  $scope.newQuestion = '';

  var askedQs = firebase.database().ref('/questions/');
  $scope.askedQuestions = $firebaseArray(askedQs);

  $scope.askedQuestions.$watch(onQuestionUpdate);

  function onQuestionUpdate(event) {
    if (event.event === 'child_changed') {
      console.log('Object Key', event.key);

      var obj = $scope.askedQuestions.$getRecord(event.key);
      if (obj.from === user.id && obj.answered === true) {
        console.log("Your question was answered");
        // $mdToast.show(
        //   $mdToast.simple()
        //   .textContent('A question has been answered!')
        //   .position('top right')
        //   .hideDelay(3000)
        // );
        recMsg();
      }
    }
  };

  function changeFavicon() {
    console.log('favoicon was changed');
    document.getElementById('favoicon').href = '/noti.ico';
  }

  function recMsg()
  {
    console.log("notiIsUp", notiIsUp)
    
    if (notiIsUp === false ){
      notiIsUp = true;
      
      var msgNoti = $mdDialog.confirm()
      .title('You got a message!')
      .targetEvent(event)
      .ok("okay");
      
      $mdDialog.show(msgNoti).then(function (result) {
        console.log("noti: showed pop up");
        //add question to answeredMsgs and delete from unanswered
        notiIsUp = false;
      });
    }
  }

  this.onDelete = function (question) {
    console.log("Trying to  DELETE: ", question);

    var delOrKeepPopOptions = $mdDialog.confirm()
      .title('Do you really want to delete this?')
      .textContent("Q: " + question.question)
      .targetEvent(event)
      .ok("Delete")
      .cancel("Keep");

    $mdDialog.show(delOrKeepPopOptions).then(function (result) {
      console.log("ANSWER: " + result);
      $scope.askedQuestions.$remove(question);

    }, function () {
      console.log("Clicked Keep");
    });
  };

  this.onAddToFAQ = function (question) {
    console.log("Trying to  ADD TO FAQ: " + question);
    var faqRef = firebase.database().ref('/faq/');
    var faq = $firebaseArray(faqRef);
    faq.$add(question);
    question.onFAQ = true;
    $scope.askedQuestions.$save(question);

    var temp = $mdDialog.alert()
      .textContent('Moved to FAQ')
      .ok('Close');
    $mdDialog.show(temp);
    $timeout(function () {
      $mdDialog.hide();
    }, 2000);

  };

  this.onRespond = function (question) {
    console.log("RESPOND was click for:" + question.question + "id: " + question.$id);

    var confirm = $mdDialog.prompt()
      .title('Question:')
      .textContent(question.question)
      .placeholder('Answer')
      .ariaLabel('Answer')
      .initialValue('')
      .required(true)
      .ok('Send')
      .cancel('Cancel');

    $mdDialog.show(confirm).then(function (result) {
      question.answer = result;
      question.answered = true;
      question.updated = firebase.database.ServerValue.TIMESTAMP;
      $scope.askedQuestions.$save(question);

      //also add to user who asked array
      console.log(question.from);
      var userMsgs = firebase.database().ref('/userMsgs/' + question.from);
      var u = $firebaseArray(userMsgs);
      u.$add({
        msg: question.answer,
        isQuestion: false,
        created: question.updated
      });


    }, function () {
      console.log("Clicked Cancel");
    });
  };

  this.onSubmitQuestion = function () {
    console.log("QUESTION " + $scope.newQuestion);
    if (!$scope.newQuestion) {
      console.log('question is required for a new question');
      return;
    }

    //add to askedQuestions
    $scope.askedQuestions.$add({
      question: $scope.newQuestion,
      answered: false,
      onFAQ: false,
      created: firebase.database.ServerValue.TIMESTAMP,
      updated: firebase.database.ServerValue.TIMESTAMP,
      from: user.id
    });

    //also add to myMsgs
    $scope.myQuestions.$add({
      msg: $scope.newQuestion,
      isQuestion: true,
      created: firebase.database.ServerValue.TIMESTAMP
    }).then(function (ref) {
      console.log(ref);
    });

    $scope.newQuestion = '';
    $scope.askForm.$setPristine();
    $scope.askForm.$setUntouched();
  };
};
