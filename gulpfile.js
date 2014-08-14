var gulp    = require('gulp')
  , concat  = require('gulp-concat')
  ;

gulp.task('default', [ 'concat' ]);
gulp.task('concat', function() {
  gulp.src([ './zbatt.py', './zblib.py' ])
    .pipe(concat('zba.py'));
  gulp.src([ './zblnk.py', './zblib.py' ])
    .pipe(concat('zbl.py'));
});
