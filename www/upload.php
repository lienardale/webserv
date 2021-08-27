<?php

// Dossier d'upload
$uploaddir = './uploads/';

echo "IN UPLOAD SCRIPT\n\n";

$env_array = getenv();
foreach ($env_array as $key=>$value)
{
    echo"$key => $value <br />\n";
}

// Sauvegarde de tous les fichiers passés en input
foreach ($_FILES as $file) {
    $uploadfile = $uploaddir . basename($file['name']);
    echo "uploading => " . $file['name'] . "\n";
    if (move_uploaded_file($file['tmp_name'], $uploadfile)){
        echo "Le fichier " . $file['name'] . " a bien été envoyé\n";
    } else {
        echo "Erreur dans l'envoi du fichier " . $file['name'] . "\n";
    }
}
?>
