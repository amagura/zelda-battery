var gulp    = require('gulp')
  , concat  = require('gulp-concat')
  , header  = require('gulp-header')
  , chmod   = require('chmod')
  ;

var head = [
  '#!/usr/bin/env julia',
  '/- filename -/',
  'zbat.jl',
  '/- copyright -/',
  'Copyright (c) 2014 Alexej Magura',
  'This file is part of Zelda-battery.',
  'Zelda-battery is free software: you can redistribute it and/or modify',
  'it under the terms of the GNU General Public License as published by',
  'the Free Software Foundation, either version 3 of the License, or',
  '(at your option) any later version.',
  '',
  'Zelda Battery is distributed in the hope that it will be useful,',
  'but WITHOUT ANY WARRANTY; without even the implied warranty of',
  'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the',
  'GNU General Public License for more details.',
  '',
  'You should have received a copy of the GNU General Public License',
  'along with Zelda Battery.  If not, see <http://www.gnu.org/licenses/>.'
]
.map(function(ln, idx, arr) {
  if (idx !== 0) ln = '# ' + ln;
  return ln;
}).join('\n');
head += '\n\n';

gulp.task('default', [ 'build', 'link' ]);

gulp.task('build', function() {
  gulp.src('./src/*.jl')
    .pipe(concat('zbat.jl'))
    .pipe(header(head))
    .pipe(gulp.dest('./'))
});

gulp.task('link', function() {
  chmod(__dirname + '/zbat.jl', 755);
});
