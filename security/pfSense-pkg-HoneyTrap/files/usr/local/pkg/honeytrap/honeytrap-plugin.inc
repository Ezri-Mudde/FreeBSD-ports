<?php
/*
 * honeytrap-plugin.inc
 *
 * part of pfSense (http://www.pfsense.org)
 * Copyright (c) 2019 DutchSec (https://dutchsec.com/)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

require_once('globals.inc');
require_once('pfsense-utils.inc');
require_once('service-utils.inc');

function honeytrap_sync_config() {
	global $g, $config;
	init_config_arr(array('installedpackages', 'honeytrap', 'config', 0));
	$gconfig = $config['installedpackages']['honeytrap']['config'][0];

	if (is_service_running('honeytrap')){
		log_error('Stopping service honeytrap');
		stop_service('honeytrap');
	}

	if (!isset($gconfig['enable']) || $gconfig['enable'] !== 'on') {
		return;
	}

	$config_file = $gconfig['config_file'];
	$stdout = "{$g['varlog_path']}/honeytrap/service.log";

	$daemon = '/usr/local/bin/honeytrap';
	$data_dir = '/usr/local/pkg/honeytrap/.data/';
	$pid = "{$g['varrun_path']}/honeytrap.pid";
	$stop = "/bin/kill -INT `/bin/cat {$pid}`";
	$start = "/usr/sbin/daemon -o {$stdout} -p {$pid} -u honeytrap {$daemon} --data {$data_dir} --config {$config_file}";
	if (isset($gconfig['truncate']) && $gconfig['truncate'] === 'on') {
		$clear = "/usr/bin/truncate -c -s 0 {$stdout}".PHP_EOL;
		$start = $clear ."\t". $start;
	}

	$write = write_rcfile(array(
		'file' => 'honeytrap-daemon.sh',
		'start' => $start,
		'stop' => $stop
	)
	);

	if (platform_booting()) {
		return;
	}

	log_error('Starting service honeytrap');
	start_service('honeytrap');
}

function honeytrap_install() {
	global $config;
	init_config_arr(array('installedpackages', 'honeytrap', 'config', 0));

	$gconfig =& $config['installedpackages']['honeytrap']['config'][0];

	$gconfig['keep'] = 'on';
	$gconfig['config_file'] = '/usr/local/pkg/honeytrap/config/default.toml';
	$gconfig['truncate'] = 'on';

	write_config('HoneyTrap: Add config');
}

function honeytrap_deinstall() {
	update_status('Removing HoneyTrap');
	if (is_service_running('honeytrap')) {
		update_status('Stopping HoneyTrap service');
		stop_service('honeytrap');
	}

	global $config;
	init_config_arr(array('installedpackages', 'honeytrap', 'config', 0));
	$gconfig =& $config['installedpackages']['honeytrap']['config'][0];

	if (!isset($gconfig['keep']) || $gconfig['keep'] != 'on') {
		update_status('Removing HoneyTrap configuration');
		unset($config['installedpackages']['honeytrap']);
		write_config('HoneyTrap: Remove config');
		unlink_if_exists('/usr/local/etc/rc.d/honeytrap-daemon.sh');
		rmdir_recursive('/usr/local/pkg/honeytrap/.data');
		rmdir_recursive('/usr/local/pkg/honeytrap/config');
	}
}
?>
