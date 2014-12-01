<?php
    error_reporting(E_ALL);
    ini_set('display_errors', 1);
    
  // require_once __DIR__ . '/vendor/ktamas77/firebase-php/lib/firebaseInterface.php';
  // require_once __DIR__ . '/vendor/ktamas77/firebase-php/lib/firebaseStub.php';
  require_once __DIR__ . '/vendor/ktamas77/firebase-php/firebaseLib.php';

  $datos = array(
    'fecha' => (new \DateTime())->format('Y-m-d H:i:s'),
    'name' => 'Pick the milk',
    'priority' => 1
  );

  // --- set up your own database here
  const DEFAULT_URL = 'https://arduinoapi.firebaseio.com/';
  const DEFAULT_TOKEN = 'F0dvtbBEJzqgcnyEKOH2c7mR5rmjWzHAsronTIhp';
  const DEFAULT_TODO_PATH = '/sample/todo';

    // Set up Firebase
    $_firebase = new Firebase(DEFAULT_URL, DEFAULT_TOKEN);

    $_firebase->set(DEFAULT_TODO_PATH, $datos);

    $response = $_firebase->get(DEFAULT_TODO_PATH);
    
    header('Content-Type: application/json');
    echo json_encode($response);
    
?>