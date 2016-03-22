# -*- mode: ruby -*-
# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
  # Default Configs
  config.vm.box = "puppetlabs/ubuntu-14.04-64-puppet"
  #config.vm.box_url = "https://cloud-images.ubuntu.com/vagrant/utopic/current/utopic-server-cloudimg-i386-vagrant-disk1.box"
  config.vm.boot_timeout = 600

  ## Librarian puppet will install manifests in puppet/code/modules
  config.librarian_puppet.puppetfile_dir = "puppet/code"
  config.librarian_puppet.placeholder_filename = ".LIB_PLACEHOLDER"
  config.librarian_puppet.use_v1_api  = '1' # Check https://github.com/rodjek/librarian-puppet#how-to-use
  config.librarian_puppet.destructive = false # Check https://github.com/rodjek/librarian-puppet#how-to-use


  ## Puppet Config - Default puppet provisioner for all VMs
  config.vm.provision "puppet" do |puppet|
    puppet.environment_path = "./puppet/code/environments"
    puppet.hiera_config_path = "./puppet/code/hiera.yaml"
    puppet.environment = "vagrant"
    puppet.options = "--confdir /vagrant/puppet"
    puppet.options = "--verbose --debug"
  end

  # VMS
  config.vm.define :pastec do |config|
    config.vm.hostname = "pastec"
    config.vm.provider :virtualbox do |v|
      v.customize ["modifyvm", :id, "--memory", "512", "--ioapic", "on" ]
      v.name = 'pastec'
    end
    config.vm.network :private_network, ip: "192.168.70.2"
    config.vm.network  :forwarded_port, guest: 4212, host: 4212, id: "Pastec", auto_correct: true
  #  config.vm.network  :forwarded_port, guest: 8088, host: 8088, id: "webserver2", auto_correct: true
  #  config.vm.network  :forwarded_port, guest: 9000, host: 9000, id: "xdebug", auto_correct: true
    # not sure on the sync yet
    config.vm.synced_folder "share/", "/home/vagrant/share", create: true, owner: "vagrant", group: "vagrant"
  end  
  ## END 

end