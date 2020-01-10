let express = require('express'),
  app = express(),
  fs = require('fs'),
  port = process.env.PORT || 3000;

if (process.env.ENV !== 'PROD') {
  app.use(require('connect-livereload')({
    port: 35729
  }));
  console.log('using livereload');
}

app.get('*', (req, res, next) => {
  console.log('Webserver Request', res.statusCode, req.method, req.url);
  next();
})

app.use(express.static('public'));
app.use(express.static('node_modules'));

app.get('/', function(request, response, next) {
  response.sendFile(__dirname + '/public/index.html');
});

app.listen(port, () => {
  console.log('Webserver started. Listening on port ' + port);
});