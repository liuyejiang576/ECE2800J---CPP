#!/bin/sh
set -e
args=$1
USER_PASSWD=$1
REPO=hzx_3/ohmyzsh
ZSH=${ZSH:-~/.oh-my-zsh}
REPO=${REPO:-ohmyzsh/ohmyzsh}
REMOTE=${REMOTE:-https://gitee.com/${REPO}.git}
BRANCH=${BRANCH:-master}
CHSH=${CHSH:-yes}
RUNZSH=${RUNZSH:-yes}
KEEP_ZSHRC=${KEEP_ZSHRC:-no}
command_exists() {
	command -v "$@" >/dev/null 2>&1
}
error() {
	echo ${RED}"Error: $@"${RESET} >&2
}
setup_color() {
	if [ -t 1 ]; then
		RED=$(printf '\033[31m')
		GREEN=$(printf '\033[32m')
		YELLOW=$(printf '\033[33m')
		BLUE=$(printf '\033[34m')
		BOLD=$(printf '\033[1m')
		RESET=$(printf '\033[m')
	else
		RED=""
		GREEN=""
		YELLOW=""
		BLUE=""
		BOLD=""
		RESET=""
	fi
}
setup_ohmyzsh() {
	umask g-w,o-w
	echo "${BLUE}Cloning Oh My Zsh...${RESET}"
	command_exists git || {
		error "git is not installed"
		exit 1
	}
	if [ "$OSTYPE" =  cygwin ] && git --version | grep -q msysgit; then
		error "Windows/MSYS Git is not supported on Cygwin"
		error "Make sure the Cygwin git package is installed and is first on the \$PATH"
		exit 1
	fi
	git clone -c core.eol=lf -c core.autocrlf=false \
		-c fsck.zeroPaddedFilemode=ignore \
		-c fetch.fsck.zeroPaddedFilemode=ignore \
		-c receive.fsck.zeroPaddedFilemode=ignore \
		--depth=1 --branch "$BRANCH" "$REMOTE" "$ZSH" || {
		error "git clone of oh-my-zsh repo failed"
		exit 1
	}
	echo
}
setup_zshrc() {
	echo "${BLUE}Looking for an existing zsh config...${RESET}"
	OLD_ZSHRC=~/.zshrc.pre-oh-my-zsh
	if [ -f ~/.zshrc ] || [ -h ~/.zshrc ]; then
		if [ $KEEP_ZSHRC =  yes ]; then
			echo "${YELLOW}Found ~/.zshrc.${RESET} ${GREEN}Keeping...${RESET}"
			return
		fi
		if [ -e "$OLD_ZSHRC" ]; then
			OLD_OLD_ZSHRC="${OLD_ZSHRC}-$(date +%Y-%m-%d_%H-%M-%S)"
			if [ -e "$OLD_OLD_ZSHRC" ]; then
				error "$OLD_OLD_ZSHRC exists. Can't back up ${OLD_ZSHRC}"
				error "re-run the installer again in a couple of seconds"
				exit 1
			fi
			mv "$OLD_ZSHRC" "${OLD_OLD_ZSHRC}"
			echo "${YELLOW}Found old ~/.zshrc.pre-oh-my-zsh." \
				"${GREEN}Backing up to ${OLD_OLD_ZSHRC}${RESET}"
		fi
		echo "${YELLOW}Found ~/.zshrc.${RESET} ${GREEN}Backing up to ${OLD_ZSHRC}${RESET}"
		mv ~/.zshrc "$OLD_ZSHRC"
	fi
	echo "${GREEN}Using the Oh My Zsh template file and adding it to ~/.zshrc.${RESET}"
	sed "/^export ZSH= / c\\
export ZSH=\"$ZSH\"
" "$ZSH/templates/zshrc.zsh-template" >  ~/.zshrc-omztemp
	mv -f ~/.zshrc-omztemp ~/.zshrc
	echo
}
setup_shell() {
	if [ $CHSH =  no ]; then
		return
	fi
	if [ "$(basename "$SHELL")" =  "zsh" ]; then
		return
	fi
	if ! command_exists chsh; then
		cat <<-EOF
			I can't change your shell automatically because this system does not have chsh.
			${BLUE}Please manually change your default shell to zsh${RESET}
		EOF
		return
	fi
	echo "${BLUE}Time to change your default shell to zsh:${RESET}"
	# Prompt for user choice on changing the default login shell
	# Check if we're running on Termux
	case "$PREFIX" in
		*com.termux*) termux=true; zsh=zsh ;;
		*) termux=false ;;
	esac
	if [ "$termux" != true ]; then
		if [ -f /etc/shells ]; then
			shells_file=/etc/shells
		elif [ -f /usr/share/defaults/etc/shells ]; then # Solus OS
			shells_file=/usr/share/defaults/etc/shells
		else
			error "could not find /etc/shells file. Change your default shell manually."
			return
		fi
		if ! zsh=$(which zsh) || ! grep -qx "$zsh" "$shells_file"; then
			if ! zsh=$(grep '^/.*/zsh$' "$shells_file" | tail -1) || [ ! -f "$zsh" ]; then
				error "no zsh binary found or not present in '$shells_file'"
				error "change your default shell manually."
				return
			fi
		fi
	fi
	if [ -n "$SHELL" ]; then
		echo $SHELL >  ~/.shell.pre-oh-my-zsh
	else
		grep "^$USER:" /etc/passwd | awk -F: '{print $7}' >  ~/.shell.pre-oh-my-zsh
	fi
    if [[ ${USER_PASSWD}x == x ]];then
        chsh -s "$zsh"
    else
        (sleep 1
        echo ${USER_PASSWD}
        ) | chsh -s "$zsh"
    fi
    if [[ ! $? -eq 0 ]]; then
		error "chsh command unsuccessful. Change your default shell manually."
	else
		export SHELL="$zsh"
		echo "${GREEN}Shell successfully changed to '$zsh'.${RESET}"
	fi

	echo
}
main() {
	if [ ! -t 0 ]; then
		RUNZSH=no
		CHSH=no
	fi
	while [ $# -gt 0 ]; do
		case $1 in
			--unattended) RUNZSH=no; CHSH=no ;;
			--skip-chsh) CHSH=no ;;
			--keep-zshrc) KEEP_ZSHRC=yes ;;
		esac
		shift
	done
	setup_color
	if ! command_exists zsh; then
		echo "${YELLOW}Zsh is not installed.${RESET} Please install zsh first."
		exit 1
	fi
	if [ -d "$ZSH" ]; then
		cat <<-EOF
			${YELLOW}You already have Oh My Zsh installed.${RESET}
			You'll need to remove '$ZSH' if you want to reinstall.
		EOF
		exit 1
	fi
	setup_ohmyzsh
	setup_zshrc
	setup_shell
	printf "$GREEN"
	cat <<-'EOF'
		         __                                     __
		  ____  /  /_     ____ ___  __  __   ____  _____ /  /_
		 /  __ \ / __ \   /  __ `__ \ /  /  /  /   /_  /  /  ___ /  __ \
		/ /_ / /  /  /  /   /  /  /  /  /  /  /_ / /     /  /_(__  ) /  /  /
		\____/_ / /_ /  /_ / /_ / /_/\__, /     /___/____/_ / /_/
		                        /____ /                       ....is now installed!
		Please look over the ~/.zshrc file to select plugins, themes, and options.
		p.s. Follow us on https://twitter.com/ohmyzsh
		p.p.s. Get stickers, shirts, and coffee mugs at https://shop.planetargon.com/collections/oh-my-zsh
	EOF
	printf "$RESET"
	if [ $RUNZSH =  no ]; then
		echo "${YELLOW}Run zsh to try it out.${RESET}"
		exit
	fi
	if [[ ${#args} -eq 1 ]];then
		exec zsh -l
	fi
}
main "$@"
