node 'pastec' {

	exec { 'refresh_cache':
	    command => 'apt-get update',
	    path    => '/usr/bin/',
	  }

	include pastec_common

	# git@github.com:phpslave/pastec.git 
	vcsrepo { '/home/vagrant/share/pastec':
		ensure   => present,
		provider => git,
		source   => 'git://github.com/phpslave/pastec.git',
	}
	
	include pastec_common::build
	include pastec_common::download_test
	# order
	Class['pastec_common'] -> Class['pastec_common::build']	-> Class['pastec_common::download_test']
}