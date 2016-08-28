use strict;
use warnings;

sub execute($) {
	my $cmd = shift;
	system($cmd);
	use Carp qw(croak);
	croak "\n\nError executing \n\t'$cmd'\n\n" if ( ( $? >> 8 ) != 0 || $? == -1 || ( $? & 127 ) != 0 );
}

my $std = '-std=c++0x';
my @compilers = ('g++-4.4', 'g++-4.7', 'g++-4.8', 'g++-4.9', 'g++-5', 'g++-6', 'icc', 'clang++-3.8');

for my $c (@compilers) {
	for my $t ('sse', 'avx', 'all') {
		print "\n$c, $t : \n";
		execute("make CXX=$c clean $t");
		execute("./test");
	}
}
