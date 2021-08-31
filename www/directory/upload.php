<?php

echo "IN UPLOAD SCRIPT\n";

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
    echo "Le fichier " . $file['name'] . " a bien été envoyé\n";
    } else {
        echo "Erreur dans l'envoi du fichier " . $file['name'] . "\n";
    }
}

// function readArrayFiles($file_post) {

//     $file_array = array();
//     $file_count = count($file_post['name']);
//     $file_keys = array_keys($file_post);

//     for ($i=0; $i<$file_count; $i++) {
//         foreach ($file_keys as $key) {
//             $file_array[$i][$key] = $file_post[$key][$i];
//         }
//     }

//     return $file_array;
// }

// if ($_FILES['upload']){
//     $file_array = readArrayFiles($_FILES['ufile']);
//     foreach ($file_array as $file) {
//         print 'File Name: ' . $file['name'];
//         print 'File Type: ' . $file['type'];
//         print 'File Size: ' . $file['size'];
//     }
// }

?>
