class pastec_common {
	$common_packages = [ 'lsof', 'htop', 'unzip', 'vim', 'mlocate', 'python-pip', 'git', 'libopencv-dev', 'libmicrohttpd-dev', 'libjsoncpp-dev', 'cmake']

	Package { ensure => 'installed' }
	package { $common_packages: }

}
