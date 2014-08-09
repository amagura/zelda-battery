var gulp    = require('gulp')
  , concat  = require('gulp-concat')
  ;

gulp.task('default', [ 'build' ]);

gulp.task('build', function() {
  gulp.src('./src/*.jl')
    .pipe(concat('zbat.jl'))
    .pipe(gulp.dest('./'))
});
