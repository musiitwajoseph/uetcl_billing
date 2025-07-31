<?php
ob_start();
session_start();
error_reporting(null);
include("../classes/init.inc");


$ad = new ActiveDirectory();

$username = 'musijo';
$password = '1616DD????';

$ad_results = $ad->login2($username, $password);  