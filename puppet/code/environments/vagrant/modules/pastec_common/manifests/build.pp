class pastec_common::build {
	file { '/home/vagrant/share/pastec/build':
    	ensure => 'directory',
  	}

  file { 'build.sh' :
    source => 'puppet:///modules/pastec_common/build.sh',
    path   => '/home/vagrant/build.sh',
    owner  => 'root',
    mode   => 'ug=rwx,o=r',
    ensure => 'present'
  }
  exec { 'build_pastec' :
    command => '/home/vagrant/build.sh',
    path    => [ '/bin' ],
    timeout => 6000
  }
}