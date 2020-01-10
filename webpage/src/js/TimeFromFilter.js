exports.TimeFromFilter = function($filter) {

  var toDate = function(time) {
    if (angular.isDate(time)) {
      time = time.getTime();
    } else if (typeof time === "string") {
      time = new Date(time).getTime();
    }
    return time;
  };

  return function(input, local, raw) {
   // console.log(input, local, raw);
    var time = input;
    if (!time) {
      return "never";
    }

    if (!local) {
      (local = Date.now());
    }

    time = toDate(time);
    local = toDate(local);

    if (typeof time !== 'number' || typeof local !== 'number') {
      return;
    }

    var
      offset = Math.abs((local - time) / 1000),
      span = [],
      MINUTE = 60,
      HOUR = 3600;
      // DAY = 86400,
      // WEEK = 604800,
      // MONTH = 2629744,
      // YEAR = 31556926,
      // DECADE = 315569260;

    if (offset <= MINUTE){
      span = ['', raw ? 'now' : 'less than a minute'];
    } else if (offset < (MINUTE * 60)) {
      span = [Math.round(Math.abs(offset / MINUTE)), 'min'];
    } else if (offset < (HOUR * 24)) {
      span = [Math.round(Math.abs(offset / HOUR)), 'hr'];
    } else {//if (offset < (DAY * 7)) {
      span = [$filter('date')(time, 'MMM d, y h:mm:ss a'), ''];
      // span = [Math.round(Math.abs(offset / DAY)), 'day'];
    // } else if (offset < (WEEK * 52)) {
    //   span = [Math.round(Math.abs(offset / WEEK)), 'week'];
    // } else if (offset < (YEAR * 10)) {
    //   span = [Math.round(Math.abs(offset / YEAR)), 'year'];
    // } else if (offset < (DECADE * 100)) {
    //   span = [Math.round(Math.abs(offset / DECADE)), 'decade'];
    // } else {
    //   span = ['', 'a long time'];
    }

    span[1] += (span[0] === 0 || span[0] > 1) ? 's' : '';
    span = span.join(' ');

    if (raw === true) {
      return span;
    }



    return (time > local) ? 'in ' + span : (offset >= (HOUR * 24)) ?  span : span + ' ago';
    // return (time <= local) ? span + ' ago' : 'in ' + span;
  };
};
