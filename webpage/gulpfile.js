'use strict';

let gulp = require('gulp'),
  babelify = require('babelify'),
  strictify = require('strictify'),
  browserify = require('browserify'),
  source = require('vinyl-source-stream'),
  buffer = require('vinyl-buffer'),
  spawn = require('child_process').spawn,
  livereload = require('gulp-livereload'),
  run = require('run-sequence'),
  gutil = require('gulp-util'),
  sourcemaps = require('gulp-sourcemaps'),
  concat = require('gulp-concat'),
  clean = require('gulp-clean'),
  less = require('gulp-less'),
  jshint = require('gulp-jshint'),
  node;

let files = {
    jshint: ['src/**/*.js'],
    livereload: ['public/build/*.js', 'public/build/*.css', 'public/**/*.html'],
    scripts: ['src/js/**/*.js'],
    scriptTop: ['src/js/main.js'],
    styles: ['src/less/*.less'],
    lessTop: ['src/less/main.less'],
    indexjs: 'index.js'
  },
  lrPort = 35729;

let restartNodeServer = () => {
  if (node) {
    node.kill('SIGTERM');
  }
  gutil.log('restarting server');
  node = spawn('node', ['--use_strict', files.indexjs], {
    stdio: 'inherit'
  })
  node.on('close', (code) => {
    if (code === 8) {
      gulp.log('Error detected, waiting for changes...');
    }
  });
}

gulp.task('watch', ['build'], () => {
  restartNodeServer();

  livereload.listen({
    start: true,
    port: lrPort
  });

  gulp.watch(files.indexjs, () => {
    restartNodeServer();
  });

  gulp.watch(files.jshint, ['jshint']);
  gulp.watch(files.scripts, ['scripts']);
  gulp.watch(files.styles, ['styles']);
  gulp.watch(files.livereload).on('change', livereload.reload);
});

gulp.task('jshint', () => {
  return gulp.src(files.jshint)
    .pipe(jshint('.jshintrc'))
    .pipe(jshint.reporter('jshint-stylish'))
});

gulp.task('clean', () => {
  return gulp.src('public/build', {
      read: false
    })
    .pipe(clean());
});

gulp.task('styles', () => {
  var l = less();
  l.on('error', function (e) {
    gutil.log(e);
    l.end();
  });
  return gulp.src(files.lessTop)
    .pipe(sourcemaps.init())
    .pipe(l)
    .on('error', gutil.log)
    .pipe(sourcemaps.write())
    .pipe(gulp.dest('public/build'));
});

gulp.task('scripts', () => {
  return browserify({
      entries: [files.scriptTop],
      debug: true
    })
    .transform(babelify, {
      presets: ['es2015']
    })
    // .transform(strictify)
    .bundle()
    .pipe(source('main.js'))
    .pipe(buffer())
    .pipe(sourcemaps.init({
      loadMaps: true
    }))
    .pipe(sourcemaps.write('.'))
    .pipe(gulp.dest('public/build'))
});

gulp.task('default', ['watch']);
gulp.task('build', (cb) => {
  run('clean', 'jshint', ['scripts', 'styles'], cb);
});

process.on('exit', () => {
  if (node) node.kill()
})
