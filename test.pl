use strict;
use warnings;

sub execute($) {
	my $cmd = shift;
	system($cmd);
	die "\n\nError executing \n\t'$cmd'\n\n" if ( ( $? >> 8 ) != 0 || $? == -1 || ( $? & 127 ) != 0 );
}

my @compilers = (@ARGV) ? @ARGV : ('g++-4.8', 'g++-4.9', 'g++-5', 'g++-6', 'clang++-3.7', 'clang++-3.8', 'clang++-3.9', 'clang++-4.0');
my @architectures = ('sse', 'avx', 'fma', 'scalar');

for my $c (@compilers) {
	for my $arch (@architectures) {
		print "$c, $arch :\n";
		execute("make CXX=$c clean $arch");
		execute("./test");
	}
}
