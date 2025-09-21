#!/bin/bash

echo Pour modifier les fichiers dans : /etc/nginx si vous utilisez linux ou WSL 
#
#echo (donc pas de folie tranquile...)

echo merci de me dire : ...
echo
echo "Quelle distro linux en utilisation ? arch/ubuntu"
echo
echo Entrez votre reponse : ...

#################################################
#	distro linux pour sudo
#################################################
#
read -e line
echo $line > tmp
if [ -f "arch" ] ; then
	su_exec="pacman"
else
	su_exec="sudo"
fi

rm tmp

#################################################
#	checker si nginx la ou pas
#################################################

file_found=$(find /etc/nginx -name nginx.conf | wc -l)
echo $file_found
if (( $file_found > 0 )) ; then
	echo file found ! You already have nginx "(noice)"
else
	$su_exec systemctl start nginx
fi

################################################
#	fonctions
################################################

script_config_file() {
	local nc="NGINC.CONF"
	echo "server {" > $nc
	echo script your file '(a basic one)' !
	echo
	sleep 1
	echo ...
	echo
	for args in \
		"	server_name	" \
		"	listen	" \
		"	host	" \
		"	root	" \
		"	error_page	"
	do
		echo $args assignment ?
		echo Enter param here :
		read -e line
		echo $args $line';' >> $nc
	done
	echo '}' >> $nc
	echo Well done !
	sleep 0.5 
	echo your file looks like : 
	cat $nc 
	echo Load data into /etc/nginx/nginx.config ? y/n
	read -e line
	echo $line > tmp
	if [ -f "y" ] ; then
		cat $nc >> /etc/nginx/nginx.config
	fi
}

set_up_default_file() {
	echo Default file setted-up !
	echo "server {
	listen 80;
	server_name exemple.com;
		
	root /var/www/html;
		
	# Règle de réécriture d'URLs
	
	location / {
      # Expédie les requests vers le répertoire principal
        	try_files $uri $uri/ =404;
	}

      # Règle de redirection, exemple : Redirect Http To Https
      # location / {
      #     return 301 https://$server_name$request_uri;
      # }

      # Autres directives de sécurité et d'optimisation
      include /etc/nginx/conf.d/*.conf;
  }" > nc
  cat nc
}
sleep 1

################################################
#	checker si argument d'entree OK
################################################

check_and_script() {
	if (( $# > 2 )) ; then
		local is=$(echo $BASH_ARGV | grep .config)
		if (( is == 0 )) ; then
			echo file doesn\'t respect [ .config ] format
			exit
		fi
	else
		echo you didn\'t give a file for argument
		echo 1\) Do you want to script your file with assistance ?
		echo 2\) Do you want to setup just a default file ?
		echo 3\) Do you want to exit the program ? ...
		echo 4\) idk other options ?
		read -e response
		echo $response > tmp
		case "$response" in
			"1") script_config_file ;;
			"2") set_up_default_file ;;
			"3") exit ;;
			*) echo "there isn't actualy other options ..." ;;
		esac
	fi
}

check_and_script

echo Verification de la syntaxe du fichier de configuration ...

is=$($su_exec nginx -t | grep failed | wc -l)

while [ true ]
do
	if (( $is == 0 )) ; then
		echo The syntax is OK !
		break
	else
		echo Let\'s retry !
		check_and_script
	fi
done

echo Restarting nginx ...

$su_exec systemctl restart nginx

echo Configuration completed !
