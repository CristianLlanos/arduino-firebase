<?php
  error_reporting(E_ALL);
  ini_set('display_errors', 1);

  require_once __DIR__ . '/vendor/ktamas77/firebase-php/firebaseLib.php';
  
  $date = new \DateTime();
  $date->setTimezone(new DateTimeZone("America/Lima"));

  $datos = array(
    'fecha' => $date->format('Y-m-d H:i:s'),
    'temperatura' => $_GET['temperatura'],
    'humedad' => $_GET['humedad'],
    'proximidad' => $_GET['proximidad']
  );

  // --- set up your own database here
  const DEFAULT_URL = 'https://arduinoapi.firebaseio.com/';
  const DEFAULT_TOKEN = 'F0dvtbBEJzqgcnyEKOH2c7mR5rmjWzHAsronTIhp';
  const DEFAULT_DATOS_PATH = '/datos';

    // Set up Firebase
    $_firebase = new Firebase(DEFAULT_URL, DEFAULT_TOKEN);

    $_firebase->push(DEFAULT_DATOS_PATH, $datos);

    $response = $_firebase->get(DEFAULT_DATOS_PATH);
    
    header('Content-Type: application/json');
    echo json_encode($response);
    
?>