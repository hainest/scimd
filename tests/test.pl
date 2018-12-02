use strict;
use warnings;
use Getopt::Long qw(GetOptions);

my $runavx512 = 0;
GetOptions('run-avx512!', \$runavx512);

sub execute($) {
	my $cmd = shift;
	system($cmd);
	die "\n\nError executing \n\t'$cmd'\n\n" if ( ( $? >> 8 ) != 0 || $? == -1 || ( $? & 127 ) != 0 );
	return 1;
}

my @compilers = (@ARGV) ? @ARGV : (
	'g++-4.8', 'g++-4.9', 'g++-5', 'g++-6', 'g++-7', 'g++-8',
	'clang++-3.7', 'clang++-3.8', 'clang++-3.9', 'clang++-4.0', 'clang++-6.0'
	);

my @architectures = ('sse', 'avx', 'fma', 'scalar', 'avx512');

for my $c (@compilers) {
	ARCH: for my $arch (@architectures) {
		print "$c, $arch : ";

		if($arch eq 'avx512') {
			$c =~ /\+\+\-(.+)/;
			my $ver = $1;
			my $unsupported_compiler =
				($c=~/g++/ && $ver < 5) ||
				($c=~/clang/ && $ver < 3.9);
			if($unsupported_compiler) {
				print "SKIPPED (unsupported compiler)\n";
				next ARCH;
			}
		}
		execute("make CXX=$c clean $arch");
		if($arch eq 'avx512' && !$runavx512) {
			print "Compiled but not run\n";
			next ARCH;
		}
		execute("./test 2>&1 >/dev/null") and print "PASSED\n";		
	}
}
