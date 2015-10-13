set nocompatible              
filetype plugin indent on   
set t_Co=256
set term=screen-256color
set number
set tabstop=4
set shiftwidth=4
set softtabstop=4
set expandtab
set wrap
set showcmd
colorscheme darkblue
map  <F2> :w <CR> :call OP() <CR> 
map! <F2> <ESC> :w <CR> :call OP() <CR> <ESC>
map  <F9> :w <CR> :call CP_R() <CR> <ESC>
map! <F9> <ESC> :w <CR> :call CP_R() <CR> <ESC>
map  <HOME> ^
map! <HOME> <ESC>^i
map  <ESC>OH <HOME>
map! <ESC>OH <HOME>
map  <END> $
map  <ESC>OF <END>
map! <ESC>OF <ESC><END>a
function CP_R()

	if( &ft == 'cpp')
		let cpl = 'g++ -w -o "%:r.exe" -std=c++11 "%"' | let exc = '"./%:r.exe"'
	elseif( &ft == 'python')
		let exc = 'python "%"'
	endif
	let pause = 'printf "Press any key to continue..." && read -n 1 && exit'
	if !exists('exc')
		echo 'Can''t compile this filetype...'
		return
	endif
	if exists('cpl')
		let cp_r = cpl . ' && time ' . exc
	else
		let cp_r = 'time ' . exc
	endif
    execute '! clear && ' . cp_r . ' && ' . pause
endfunction

function OP()
	execute '!$COLORTERM -x gedit ' . "%" . ";"
endfunction

