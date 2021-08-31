<?php

// Dossier d'upload
$uploaddir = './uploads/';

echo "IN UPLOAD SCRIPT";

echo "BEFORE SAVE";
// Sauvegarde de tous les fichiers passés en input
foreach ($_FILES as $file) {
    $uploadfile = $uploaddir . basename($file['name']);
    if (move_uploaded_file($file['tmp_name'], $uploadfile)){
        echo "Le fichier " . $file['name'] . " a bien été envoyé\n";
    } else {
        echo "Erreur dans l'envoie du fichier " . $file['name'] . "\n";
    }
}
echo "AFTER SAVE";
?>
