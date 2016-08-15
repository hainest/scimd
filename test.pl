use strict;
use warnings;

sub execute($) {
	my $cmd = shift;
	system($cmd);
	use Carp qw(croak);
	croak "\n\nError executing \n\t'$cmd'\n\n" if ( ( $? >> 8 ) != 0 || $? == -1 || ( $? & 127 ) != 0 );
}

my %config = (
	'g++-4.4' => '-std=c++0x',
	'g++-4.7' => '-std=c++11',
	'g++-4.8' => '-std=c++11',
	'g++-4.9' => '-std=c++14',
	'g++-5'   => '-std=c++14',
	'g++-6'   => '-std=c++14',
	'icc'     => '-std=c++11',
	'clang++-3.8' => '-std=c++11'
);

for my $c (keys %config) {
	for my $i (1..4) {
		print "$c, $i : ";
		execute("$c $config{$c} -DVERSION=$i -O3 -o test test.cpp");
		execute("./test 16.0 4.0");
	}
}