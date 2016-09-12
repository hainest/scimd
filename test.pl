use strict;
use warnings;
use Getopt::Long qw(GetOptions);

my $legacy = 0;
GetOptions('legacy'=>\$legacy);

sub execute($) {
	my $cmd = shift;
	system($cmd);
	use Carp qw(croak);
	croak "\n\nError executing \n\t'$cmd'\n\n" if ( ( $? >> 8 ) != 0 || $? == -1 || ( $? & 127 ) != 0 );
}

my $std = '-std=c++11';
my @compilers = ('g++-4.8', 'g++-4.9', 'g++-5', 'g++-6', 'icc', 'clang++-3.7', 'clang++-3.8');
my @test_architectures = ('sse', 'avx');

# Don't run the scalar tests for the legacy code
push @test_architectures, 'all' if !$legacy;

if ($legacy) {
	$std = 'c++0x';
	unshift @compilers, ('g++-4.4', 'g++-4.5', 'g++-4.6', 'g++-4.7');
}

for my $c (@compilers) {
	for my $t (@test_architectures) {
		print "$c, $t : ";
		execute("make CXX=$c clean $t");
		execute("./test");
	}
}
