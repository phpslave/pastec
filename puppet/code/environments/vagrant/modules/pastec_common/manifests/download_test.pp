class pastec_common::download_test {

  file { 'download_test_dat.sh' :
    source => 'puppet:///modules/pastec_common/download_test_dat.sh',
    path   => '/home/vagrant/download_test_dat.sh',
    owner  => 'root',
    mode   => 'ug=rwx,o=r',
    ensure => 'present'
  }
  exec { 'download_test_dat' :
    command => '/home/vagrant/download_test_dat.sh',
    path    => [ '/bin' ],
    timeout => 6000
  }
}