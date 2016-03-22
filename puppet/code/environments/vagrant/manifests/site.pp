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
	
	include plastec_common::build
	
	# order
	Class['pastec_common'] -> Class['plastec_common::build']	
}