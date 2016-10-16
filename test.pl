use strict;
use warnings;
use Getopt::Long qw(GetOptions);

my $legacy = 0;
GetOptions('legacy'=>\$legacy);

sub execute($) {
	my $cmd = shift;
	system($cmd);
	die "\n\nError executing \n\t'$cmd'\n\n" if ( ( $? >> 8 ) != 0 || $? == -1 || ( $? & 127 ) != 0 );
}

my @compilers = ('g++-4.8', 'g++-4.9', 'g++-5', 'g++-6', 'icc', 'clang++-3.7', 'clang++-3.8', 'clang++-3.9');
my @test_architectures = ('sse', 'avx', 'scalar');

for my $c (@compilers) {
	for my $t (@test_architectures) {
		print "$c, $t : ";
		execute("make CXX=$c clean $t");
		execute("./test");
	}
}
