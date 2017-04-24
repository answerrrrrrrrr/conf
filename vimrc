"==========================================
"               Air9.vimrc
"
" Based on: https://github.com/wklken/k-vim
"
" Index:
"       -> Plugins 插件
"       -> General Settings 基础设置
"       -> Display Settings 展示/排版等界面格式设置
"       -> Encode Settings 文件编码设置
"       -> Others 其它配置
"       -> HotKey Settings  自定义快捷键
"       -> FileType Settings  针对文件类型的设置
"       -> Theme Settings  主题设置
"
"==========================================

"==========================================
" Plugins 插件
"==========================================

" 修改leader键
let mapleader   = ','
let g:mapleader = ','

"R0 开启语法高亮
syntax on


" vim-plug
call plug#begin('~/.vim/plugged')

Plug 'crusoexia/vim-monokai'
Plug 'bling/vim-airline'
Plug 'nathanaelkane/vim-indent-guides'
Plug 'junegunn/vim-easy-align'
Plug 'scrooloose/syntastic'
Plug 'scrooloose/nerdtree'
Plug 'scrooloose/nerdcommenter'
"Plug 'Xuyuanp/nerdtree-git-plugin'
Plug 'majutsushi/tagbar'
Plug 'easymotion/vim-easymotion' " quick location
Plug 'terryma/vim-multiple-cursors'
Plug 'vim-scripts/a.vim',  { 'for': ['c', 'cpp'] } " switch source and header
Plug 'aceofall/gtags.vim', { 'for': ['c', 'cpp'] } " gtags instead of cscope
Plug 'sjl/gundo.vim'
Plug 'terryma/vim-expand-region'
Plug 'kien/rainbow_parentheses.vim'
Plug 'kien/ctrlp.vim' " file fuzz
Plug 'dyng/ctrlsf.vim' " global search results with context
Plug 'kshenoy/vim-signature' " enhanced marks
"Plug 'tpope/vim-surround'
"Plug 'tpope/vim-repeat'
Plug 'tpope/vim-obsession' " save session of buffers and tabs
Plug 'thinca/vim-quickrun'
Plug 'Shougo/neocomplete'
Plug 'iamcco/markdown-preview.vim'
Plug 'plasticboy/vim-markdown'
Plug 'godlygeek/tabular'


call plug#end()


"==========================================
" airline

"let g:airline_theme="ravenpower"
"
"let g:airline_powerline_fonts = 1
"let g:airline_detect_iminsert=1
"let g:airline_left_sep=''
"let g:airline_right_sep=''

let g:airline#extensions#tabline#enabled = 1
let g:airline#extensions#tabline#buffers_label = 'b'
let g:airline#extensions#tabline#buffer_nr_show = 1
let g:airline#extensions#tabline#tabs_label = 't'

"let g:airline#extensions#obsession#enabled = 1
"let g:airline#extensions#obsession#indicator_text = '☯'

" 关闭状态显示空白符号计数
let g:airline#extensions#whitespace#enabled = 0
let g:airline#extensions#whitespace#symbol = '!'

"let g:airline#extensions#ctrlspace#enabled = 1
"let g:CtrlSpaceStatuslineFunction = "airline#extensions#ctrlspace#statusline()"

"let g:airline#extensions#ycm#enabled = 1



" vim-indent-guides
nmap <silent> <Leader>i <Plug>IndentGuidesToggle
let g:indent_guides_enable_on_vim_startup=0
let g:indent_guides_start_level=2
let g:indent_guides_guide_size=1
let g:indent_guides_exclude_filetypes = ['help', 'nerdtree']
let g:indent_guides_auto_colors = 0
autocmd VimEnter * :hi IndentGuidesOdd  guibg=grey10 ctermbg=233
autocmd VimEnter * :hi IndentGuidesEven guibg=grey15 ctermbg=236



" easy-align
vmap <Enter> <Plug>(EasyAlign)



" syntastic
let g:syntastic_always_populate_loc_list = 1
let g:syntastic_auto_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_check_on_wq = 0

let g:syntastic_error_symbol = '✗'
let g:syntastic_warning_symbol = '⚠'
let g:syntastic_enable_balloons = 1

"let g:syntastic_cpp_include_dirs = ['/usr/include/']
"let g:syntastic_cpp_remove_include_errors = 1
"let g:syntastic_cpp_check_header = 1
"let g:syntastic_cpp_compiler = 'clang++'
"let g:syntastic_cpp_compiler_options = '-std=c++1z '



" The-NERD-tree
nnoremap <leader>n :NERDTreeToggle<CR>
let NERDTreeHighlightCursorline=1
let NERDTreeIgnore=[ '\.pyc$', '\.pyo$', '\.obj$', '\.o$', '\.so$', '\.egg$', '^\.git$', '^\.svn$', '^\.hg$', '\~$', 'node_modules']
" close vim if the only window left open is a NERDTree
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTreeType") && b:NERDTreeType == "primary") | q | end
" 当不带参数打开Vim时自动加载项目树
autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * if argc() == 0 && !exists("s:std_in") | NERDTree | endif
" 不显示项目树上额外的信息，例如帮助、提示什么的
let NERDTreeMinimalUI=1



" EasyMotion
"<Leader><Leader>w
"<Leader><Leader>e
"<Leader><Leader>f
"<Leader><Leader>b
"<Leader><Leader>h
"<Leader><Leader>k
"...
":h easymotion



" vim-multiple-cursors
"let g:multi_cursor_use_default_mapping=0
" Default mapping
"let g:multi_cursor_next_key='<C-n>'
"let g:multi_cursor_prev_key='<C-p>'
"let g:multi_cursor_skip_key='<C-x>'
"let g:multi_cursor_quit_key='<Esc>'

" Called once right before you start selecting multiple cursors
function! Multiple_cursors_before()
  if exists(':NeoCompleteLock')==2
    exe 'NeoCompleteLock'
  endif
endfunction

" Called once only when the multiple selection is canceled (default <Esc>)
function! Multiple_cursors_after()
  if exists(':NeoCompleteUnlock')==2
    exe 'NeoCompleteUnlock'
  endif
endfunction


" tagbar
nmap <leader>tb :TagbarToggle<CR>



" a.vim
"头/源文件切换命令
    ":A 头文件／源文件切换
    ":AS 分割窗后并切换头/源文件(切割为上下两个窗口)
    ":AV 垂直切割窗口后切换头/源文件(切割为左右两个窗口)
    ":AT 新建Vim标签式窗口后切换
    ":AN 在多个匹配文件间循环切换
"将光标所在处单词作为文件名打开
    ":IH 切换至光标所在文件
    ":IHS 分割窗口后切换至光标所在文件(指将光标所在处单词作为文件名打开)
    ":IHV 垂直分割窗口后切换
    ":IHT 新建标签式窗口后切换
    ":IHN 在多个匹配文件间循环切换
"快捷键操作
    "<Leader>ih 切换至光标所在文件*
    "<Leader>is 切换至光标所在处(单词所指)文件的配对文件(如光标所在处为foo.h，则切换至foo.c/foo.cpp...)
    "<Leader>ihn 在多个匹配文件间循环切换



" gtags
set cscopetag " 使用 cscope 作为 tags 命令
set cscopeprg='gtags-cscope' " 使用 gtags-cscope 代替 cscope

let GtagsCscope_Auto_Load = 1
let CtagsCscope_Auto_Map = 1
let GtagsCscope_Quiet = 1

"Find this C symbol:
"Find this global definition:
"Find functions called by this function:
"Find functions calling this function:
"Find this egrep pattern:
"Find this file:
"Find files #including this file:
"Find assignments to this symbol:
nmap <leader>gs :cs find s <C-R>=expand('<cword>')<CR><CR>
nmap <leader>gg :cs find g <C-R>=expand('<cword>')<CR><CR>
nmap <leader>gc :cs find c <C-R>=expand('<cword>')<CR><CR>
nmap <leader>gt :cs find t <C-R>=expand('<cword>')<CR><CR>
nmap <leader>ge :cs find e <C-R>=expand('<cword>')<CR><CR>
nmap <leader>gf :cs find f <C-R>=expand('<cfile>')<CR><CR>
nmap <leader>gi :cs find i ^<C-R>=expand('<cfile>')<CR>$<CR>
nmap <leader>gd :cs find d <C-R>=expand('<cword>')<CR><CR>



" gundo
nmap <leader>u :GundoToggle<CR>



" vim-expand-region
" default keymaps + and _



" RainbowParentheses
autocmd VimEnter * RainbowParenthesesActivate
autocmd Syntax * RainbowParenthesesLoadRound
autocmd Syntax * RainbowParenthesesLoadSquare
autocmd Syntax * RainbowParenthesesLoadBraces



" vim-signature
"   m[a-zA-Z]   打标签
"   '[a-zA-Z]   跳转到标签位置
"   '.          最后一次变更的地方
"   ''          跳回来的地方(最近两个位置跳转)
"   m<space>    去除所有标签



" ctrlsf
let g:ctrlsf_ackprg = 'ag'
nmap     <C-F>f <Plug>CtrlSFPrompt
vmap     <C-F>f <Plug>CtrlSFVwordPath
vmap     <C-F>F <Plug>CtrlSFVwordExec
nmap     <C-F>n <Plug>CtrlSFCwordPath 
nmap     <C-F>p <Plug>CtrlSFPwordPath
nnoremap <C-F>t :CtrlSFToggle<CR>
inoremap <C-F>t <Esc>:CtrlSFToggle<CR>
" press p in results window to preview



" vim-surround
"let g:surround_no_mappingsg = 1
"Normal mode
"-----------
"ds  - delete a surrounding
"cs  - change a surrounding
"ys  - add a surrounding
"yS  - add a surrounding and place the surrounded text on a new line + indent it
"yss - add a surrounding to the whole line
"ySs - add a surrounding to the whole line, place it on a new line + indent it
"ySS - same as ySs
"
"示例:
"    # 替换: cs"'
"    "Hello world!" -> 'Hello world!'
"    
"    # 替换-标签(t=tag): cst"
"    <a>abc</a>  -> "abc"
"    
"    cst<html>
"    <a>abc</a>  -> <html>abc</html>
"    
"    # 删除: ds"
"    "Hello world!" -> Hello world!
"    
"    # 添加(ys=you surround): ysiw"
"    Hello -> "Hello"
"    
"    # 添加: csw"
"    Hello -> "Hello"
"    
"    # 添加-整行: yss"
"    Hello world -> "Hello world"
"    
"    # ySS"
"    Hello world ->
"    "
"        hello world
"    "
"    
"    # 添加-两个词: veeS"
"    hello world -> "hello world"
"    
"    # 添加-当前到行尾: ys$"
"    
"    # 左符号/右符号 => 带不带空格
"    cs([
"    (hello) -> [ hello ]
"    
"    cs(]
"    (hello) -> [hello]
"
"Visual mode
"-----------
"S   - in visual mode, add a surrounding
"gS   - in visual mode, add a surrounding but place text on new line + indent it

"Insert mode
"-----------
"<CTRL-s> - in insert mode, add a surrounding
"<CTRL-s><CTRL-s> - in insert mode, add a new line + surrounding + indent
"<CTRL-g>s - same as <CTRL-s>
"<CTRL-g>S - same as <CTRL-s><CTRL-s>



" vim-repeat
" repeat vim-surround by .



" vim-obsession
" :mks



" quickrun
let g:quickrun_config = {
            \   "_" : {
            \       "outputter" : "message",
            \   },
            \}

"let g:quickrun_no_default_key_mappings = 1
nmap <Leader>r <Plug>(quickrun)
"map <F10> :QuickRun<CR>



" neocomplete
"Note: This option must set it in .vimrc(_vimrc).  NOT IN .gvimrc(_gvimrc)!
" Disable AutoComplPop.
let g:acp_enableAtStartup = 0
" Use neocomplete.
let g:neocomplete#enable_at_startup = 1
" Use smartcase.
let g:neocomplete#enable_smart_case = 1
" Set minimum syntax keyword length.
let g:neocomplete#sources#syntax#min_keyword_length = 3
let g:neocomplete#lock_buffer_name_pattern = '\*ku\*'

" Define dictionary.
let g:neocomplete#sources#dictionary#dictionaries = {
    \ 'default' : '',
    \ 'vimshell' : $HOME.'/.vimshell_hist',
    \ 'scheme' : $HOME.'/.gosh_completions'
        \ }

" Define keyword.
if !exists('g:neocomplete#keyword_patterns')
    let g:neocomplete#keyword_patterns = {}
endif
let g:neocomplete#keyword_patterns['default'] = '\h\w*'

" Plugin key-mappings.
inoremap <expr><C-g>     neocomplete#undo_completion()
inoremap <expr><C-l>     neocomplete#complete_common_string()

" Recommended key-mappings.
" <CR>: close popup and save indent.
inoremap <silent> <CR> <C-r>=<SID>my_cr_function()<CR>
function! s:my_cr_function()
  return (pumvisible() ? "\<C-y>" : "" ) . "\<CR>"
  " For no inserting <CR> key.
  "return pumvisible() ? "\<C-y>" : "\<CR>"
endfunction
" <TAB>: completion.
inoremap <expr><TAB>  pumvisible() ? "\<C-n>" : "\<TAB>"
" <C-h>, <BS>: close popup and delete backword char.
"inoremap <expr><C-h> neocomplete#smart_close_popup()."\<C-h>"
"inoremap <expr><BS> neocomplete#smart_close_popup()."\<C-h>"
" Close popup by <Space>.
"inoremap <expr><Space> pumvisible() ? "\<C-y>" : "\<Space>"

" AutoComplPop like behavior.
"let g:neocomplete#enable_auto_select = 1

" Shell like behavior(not recommended).
"set completeopt+=longest
"let g:neocomplete#enable_auto_select = 1
"let g:neocomplete#disable_auto_complete = 1
"inoremap <expr><TAB>  pumvisible() ? "\<Down>" : "\<C-x>\<C-u>"

" Enable omni completion.
autocmd FileType css setlocal omnifunc=csscomplete#CompleteCSS
autocmd FileType html,markdown setlocal omnifunc=htmlcomplete#CompleteTags
autocmd FileType javascript setlocal omnifunc=javascriptcomplete#CompleteJS
autocmd FileType python setlocal omnifunc=pythoncomplete#Complete
autocmd FileType xml setlocal omnifunc=xmlcomplete#CompleteTags

" Enable heavy omni completion.
if !exists('g:neocomplete#sources#omni#input_patterns')
  let g:neocomplete#sources#omni#input_patterns = {}
endif
"let g:neocomplete#sources#omni#input_patterns.php = '[^. \t]->\h\w*\|\h\w*::'
"let g:neocomplete#sources#omni#input_patterns.c = '[^.[:digit:] *\t]\%(\.\|->\)'
"let g:neocomplete#sources#omni#input_patterns.cpp = '[^.[:digit:] *\t]\%(\.\|->\)\|\h\w*::'

" For perlomni.vim setting.
" https://github.com/c9s/perlomni.vim
let g:neocomplete#sources#omni#input_patterns.perl = '\h\w*->\h\w*\|\h\w*::'



" MarkdownPreview
let g:mkdp_path_to_chrome = "open -a Safari"
" path to the chrome or the command to open chrome(or other modern browsers)

let g:mkdp_auto_start = 0
" set to 1, the vim will open the preview window once enter the markdown
" buffer

let g:mkdp_auto_open = 0
" set to 1, the vim will auto open preview window when you edit the
" markdown file

let g:mkdp_auto_close = 1
" set to 1, the vim will auto close current preview window when change
" from markdown buffer to another buffer

let g:mkdp_refresh_slow = 0
" set to 1, the vim will just refresh markdown when save the buffer or
" leave from insert mode, default 0 is auto refresh markdown as you edit or
" move the cursor

let g:mkdp_command_for_global = 0
" set to 1, the MarkdownPreview command can be use for all files,
" by default it just can be use in markdown file







"==========================================
" General Settings 基础设置
"==========================================

"Mac/Win clipboard
"set clipboard+=unnamed
"建议使用 :set 的 += 和 -= 特性，这可以避免未来版本增加新的标志位时出现的问题

" history存储容量
set history=2000

"检测文件类型
filetype on
"针对不同的文件类型采用不同的缩进格式
filetype indent on
"允许插件
filetype plugin on
"启动自动补全
filetype plugin indent on

set autoread          " 文件修改之后自动载入。
set shortmess=atI       " 启动的时候不显示那个援助索马里儿童的提示

" 备份,到另一个位置. 防止误删, 目前是取消备份
"set backup
"set backupext=.bak
"set backupdir=/tmp/vimbk/

" 取消备份。 视情况自己改
set nobackup
" 关闭交换文件
set noswapfile


"create undo file
if has('persistent_undo')
  set undolevels=1000         " How many undos
  set undoreload=10000        " number of lines to save for undo
  set undofile                " So is persistent undo ...
  set undodir=/tmp/vimundo/
endif


"set cursorcolumn
set cursorline

"退出vim后，内容显示在终端屏幕, 可以用于查看和复制
"set t_ti= t_te=

"修复ctrl+m 多光标操作选择的bug，但是改变了ctrl+v进行字符选中时将包含光标下的字符
"set selection=exclusive
set selection=inclusive
"set selectmode=mouse,key

" No annoying sound on errors
" 去掉输入错误的提示声音
set title                " change the terminal's title
set novisualbell         " don't beep
set noerrorbells         " don't beep
set t_vb=
set tm=500

" Remember info about open buffers on close"
set viminfo^=%

" For regular expressions turn magic on
"set magic

" Configure backspace so it acts as it should act
set backspace=eol,start,indent
set whichwrap+=<,>,h,l

"==========================================
" Display Settings 展示/排版等界面格式设置
"==========================================

"显示当前的行号列号：
set ruler
""在状态栏显示正在输入的命令
set showcmd
" 左下角显示当前vim模式
set showmode

" 在上下移动光标时，光标的上方或下方至少会保留显示的行数
set scrolloff=5

" set winwidth=79

" 命令行（在状态行下）的高度，默认为1，这里是2
set statusline=%<%f\ %h%m%r%=%k[%{(&fenc==\"\")?&enc:&fenc}%{(&bomb?\",BOM\":\"\")}]\ %-14.(%l,%c%V%)\ %P
" Always show the status line - use 2 lines for the status bar
set laststatus=2

"显示行号：
set number
" 取消换行。
"set nowrap

" 括号配对情况,跳转并高亮一下匹配的括号
set showmatch
" How many tenths of a second to blink when matching brackets
set matchtime=2

"设置文内智能搜索提示
" 高亮search命中的文本。
set hlsearch
" 打开增量搜索模式,随着键入即时搜索
set incsearch
" 搜索时忽略大小写
set ignorecase
" 有一个或以上大写字母时仍大小写敏感
set smartcase     " ignore case if search pattern is all lowercase, case-sensitive otherwise


" 代码折叠
set foldenable
" 折叠方法
" manual    手工折叠
" indent    使用缩进表示折叠
" expr      使用表达式定义折叠
" syntax    使用语法定义折叠
" diff      对没有更改的文本进行折叠
" marker    使用标记进行折叠, 默认标记是 {{{ 和 }}}
set foldmethod=indent
set foldlevel=99
" 代码折叠自定义快捷键
let g:FoldMethod = 0
map <leader>zz :call ToggleFold()<cr>
fun! ToggleFold()
    if g:FoldMethod == 0
        exe "normal! zM"
        let g:FoldMethod = 1
    else
        exe "normal! zR"
        let g:FoldMethod = 0
    endif
endfun
nnoremap <space> @=((foldclosed(line('.')) < 0) ? 'zc' : 'zo')<CR> " space toggle

" 缩进配置
set smartindent   " Smart indent
set autoindent    " 打开自动缩进
" never add copyindent, case error   " copy the previous indentation on autoindenting

" tab相关变更
set tabstop=4     " 设置Tab键的宽度        [等同的空格个数]
set shiftwidth=4  " 每一次缩进对应的空格数
set softtabstop=4 " 按退格键时可以一次删掉 4 个空格
set smarttab      " insert tabs on the start of a line according to shiftwidth, not tabstop 按退格键时可以一次删掉 4 个空格
set expandtab     " 将Tab自动转化成空格    [需要输入真正的Tab键时，使用 Ctrl+V + Tab]
set shiftround    " 缩进时，取整 use multiple of shiftwidth when indenting with '<' and '>'

" tab和space可见
"set list listchars=tab:→\ ,trail:·

" A buffer becomes hidden when it is abandoned
"set hidden

" fast terminal print
set ttyfast

" 00x增减数字时使用十进制
set nrformats=


" 相对行号      行号变成相对，可以用 nj  nk   进行跳转 5j   5k 上下跳5行
set relativenumber number
au FocusLost * :set norelativenumber number
au FocusGained * :set relativenumber
" 插入模式下用绝对行号, 普通模式下用相对
autocmd InsertEnter * :set norelativenumber number
autocmd InsertLeave * :set relativenumber
function! NumberToggle()
  if(&relativenumber == 1)
    set norelativenumber number
  else
    set relativenumber
  endif
endfunc
nnoremap <C-n> :call NumberToggle()<cr>


"==========================================
" Encode Settings 文件编码,格式
"==========================================
" 设置新文件的编码为 UTF-8
set encoding=utf-8
" 自动判断编码时，依次尝试以下编码：
set fileencodings=ucs-bom,utf-8,cp936,gb18030,big5,euc-jp,euc-kr,latin1
set helplang=cn
"set langmenu=zh_CN.UTF-8
"set enc=2byte-gb18030
" 下面这句只影响普通模式 (非图形界面) 下的 Vim。
set termencoding=utf-8

" Use Unix as the standard file type
set ffs=unix,dos,mac

" 如遇Unicode值大于255的文本，不必等到空格再折行。
set formatoptions+=m
" 合并两行中文时，不在中间加空格：
set formatoptions+=B


"==========================================
" others 其它设置
"==========================================
"autocmd! bufwritepost _vimrc source % " vimrc文件修改之后自动加载。 windows。
autocmd! bufwritepost .vimrc source % " vimrc文件修改之后自动加载。 linux。

" 自动补全配置
"让Vim的补全菜单行为与一般IDE一致(参考VimTip1228)
set completeopt=menu

" cmode autocomplete
set wildmode=list:longest
"set wildmenu
set wildignore=*.o,*~,*.swp,*.bak,*.pyc,.svn

"离开插入模式后自动关闭预览窗口
autocmd InsertLeave * if pumvisible() == 0|pclose|endif
"回车即选中当前项
inoremap <expr> <CR>       pumvisible() ? "\<C-y>" : "\<CR>"

"上下左右键的行为 会显示其他信息
inoremap <expr> <Down>     pumvisible() ? "\<C-n>" : "\<Down>"
inoremap <expr> <Up>       pumvisible() ? "\<C-p>" : "\<Up>"
inoremap <expr> <PageDown> pumvisible() ? "\<PageDown>\<C-p>\<C-n>" : "\<PageDown>"
inoremap <expr> <PageUp>   pumvisible() ? "\<PageUp>\<C-p>\<C-n>" : "\<PageUp>"

" 打开时自动跳转至上次光标位置
" if this not work ,make sure .viminfo is writable for you
if has("autocmd")
  au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
endif
"au BufReadPost * exe "normal! g`\""

"==========================================
" HotKey Settings  自定义快捷键设置
"==========================================

" 主要按键重定义

" Map ; to : and save a million keystrokes
nnoremap ; :

" Esc & save
inoremap jj <Esc>:w<CR>zz

" 关闭方向键, 强迫自己用 hjkl
map <Left> <Nop>
map <Right> <Nop>
map <Up> <Nop>
map <Down> <Nop>

"Treat long lines as break lines (useful when moving around in them)
"se swap之后，同物理行上线直接跳
nnoremap k gk
nnoremap gk k
nnoremap j gj
nnoremap gj j



" F1 - F6 设置



" F1 关闭鼠标和行号, 用于复制
"set mouse-=a             " 鼠标暂不启用, 键盘党....
set mouse=a                 " Automatically enable mouse usage

noremap <F1> :call SetMouse()<CR>

function! SetMouse()
    if(&mouse == 'a')
        set mouse=
    else
        set mouse=a
    endif
    call HideNumber()
endfunc

function! HideNumber()
  if(&relativenumber == &number)
    set relativenumber! number!
  elseif(&number)
    set number!
  else
    set relativenumber!
  endif
  set number?
endfunc


" F2 set paste
set pastetoggle=<F2>
" disbale paste mode when leaving insert mode
au InsertLeave * set nopaste



" 显示不可见字符
noremap <F3> :set list! list?<CR>

" 换行
noremap <F4> :set wrap! wrap?<CR>

" 语法高亮
noremap <F6> :exec exists('syntax_on') ? 'syn off' : 'syn on'<CR>

" NeoCompleteToggle
noremap <F7> :NeoCompleteToggle<CR>

" SyntasticToggle
noremap <F8> :SyntasticToggleMode<CR>

" MarkdownPreview
noremap <silent> <F9> :MarkdownPreview<CR>
"noremap <silent> <F9> :MarkdownPreviewStop<CR>:MarkdownPreview<CR>

" 分屏窗口移动
"map <C-j> <C-W>j
"map <C-k> <C-W>k
"map <C-h> <C-W>h
"map <C-l> <C-W>l

" 插入模式操作(SecureCRT需设置“Backspace send delete”)
inoremap <C-h> <left>
inoremap <C-j> <down>
inoremap <C-k> <up>
inoremap <C-l> <right>
inoremap <C-o> <Esc>o

" home & end
nnoremap <C-a> ^
nnoremap <C-e> g_
cnoremap <C-a> <Home>
inoremap <C-a> <Home>
cnoremap <C-e> <End>
inoremap <C-e> <End>


" 搜索相关
" Map <Space> to / (search) and Ctrl-<Space> to ? (backwards search)
"map <space> /
" 进入搜索Use sane regexes"
"nnoremap / /\v
"vnoremap / /\v

"Keep search pattern at the center of the screen."
nnoremap <silent> n nzz
nnoremap <silent> N Nzz
nnoremap <silent> * *zz
nnoremap <silent> # #zz
nnoremap <silent> g* g*zz

" switch # *
"nnoremap # *
"nnoremap * #

" for # indent, python文件中输入新行时#号注释不切回行首
autocmd BufNewFile,BufRead *.py inoremap # X<c-h>#

" 去掉搜索高亮
noremap <silent><leader>/ :nohls<CR>

" --------tab/buffer相关
"Use arrow key to change buffer"
" :b1 :b2   :bf :bl
nnoremap [b :bprevious<cr>
nnoremap ]b :bnext<cr>
noremap <left> :bp<CR>
noremap <right> :bn<CR>


" tab 操作
" TODO: ctrl + n 变成切换tab的方法
" http://vim.wikia.com/wiki/Alternative_tab_navigation
" http://stackoverflow.com/questions/2005214/switching-to-a-particular-tab-in-vim
"map <C-2> 2gt
map <leader>tf :tabfirst<cr>
map <leader>tl :tablast<cr>
map <leader>tn :tabnext<cr>
map <leader>tp :tabprev<cr>
map <leader>te :tabedit<cr>
map <leader>tc :tabclose<cr>
map <leader>tm :tabm<cr>

" 新建tab  Ctrl+t
nnoremap <C-t>     :tabnew<CR>
inoremap <C-t>     <Esc>:tabnew<CR>
nnoremap <C-S-tab> :tabprevious<CR>
nnoremap <C-tab>   :tabnext<CR>
inoremap <C-S-tab> <Esc>:tabprevious<CR>i
inoremap <C-tab>   <Esc>:tabnext<CR>i

" normal模式下切换到确切的tab
noremap <leader>1 1gt
noremap <leader>2 2gt
noremap <leader>3 3gt
noremap <leader>4 4gt
noremap <leader>5 5gt
noremap <leader>6 6gt
noremap <leader>7 7gt
noremap <leader>8 8gt
noremap <leader>9 9gt
noremap <leader>0 :tablast<cr>

" Toggles between the active and last active tab "
" The first tab is always 1 "
let g:last_active_tab = 1
" nnoremap <leader>gt :execute 'tabnext ' . g:last_active_tab<cr>
" nnoremap <silent> <c-o> :execute 'tabnext ' . g:last_active_tab<cr>
" vnoremap <silent> <c-o> :execute 'tabnext ' . g:last_active_tab<cr>
nnoremap <silent> <leader>tt :execute 'tabnext ' . g:last_active_tab<cr>
vnoremap <silent> <leader>tt :execute 'tabnext ' . g:last_active_tab<cr>
autocmd TabLeave * let g:last_active_tab = tabpagenr()


" ------- 选中及操作改键

" {}
inoremap <Leader>b {}<LEFT><CR><ESC>O

" move lines like eclipse
nnoremap <C-S-Up> ddkP
inoremap <C-S-Up> <Esc>ddkPi
nnoremap <C-S-Down> ddp
inoremap <C-S-Down> <Esc>ddpi

" reselect visual block after indent/outdent.调整缩进后自动选中，方便再次操作
vnoremap < <gv
vnoremap > >gv

" y$ -> Y Make Y behave like other capitals
map Y y$

" clipboard I/O
vnoremap <leader>y "+y
nnoremap <leader>y "+y
nnoremap <leader>yy "+yy
nnoremap <leader>p "+p
nnoremap <leader>P "+P

" select all to clipboard
"map <Leader>sa ggVG"+

" auto jump to end of select
" vnoremap <silent> y y`]
" vnoremap <silent> p p`]
" nnoremap <silent> p p`]

" select block
"nnoremap <leader>bp V`{
"nnoremap <leader>bn V`}

" w!! to sudo & write a file
cmap w!! w !sudo tee >/dev/null %

" Quickly close the current window
nnoremap <leader>q :q<CR>
" Quickly save the current file
nnoremap <leader>w :w<CR>

" Swap implementations of ` and ' jump to markers
" By default, ' jumps to the marked line, ` jumps to the marked line and
" column, so swap them
"nnoremap ' `
"nnoremap ` '

" remap U to <C-r> for easier redo
nnoremap U <C-r>

" Quickly edit/reload the vimrc file
nmap <silent> <leader>vv :e $DOTFILES/vimrc<CR>
nmap <silent> <leader>sv :so $MYVIMRC<CR>

" Automatically set paste mode in Vim when pasting in insert mode
function! WrapForTmux(s)
  if !exists('$TMUX')
    return a:s
  endif

  let tmux_start = "\<Esc>Ptmux;"
  let tmux_end = "\<Esc>\\"

  return tmux_start . substitute(a:s, "\<Esc>", "\<Esc>\<Esc>", 'g') . tmux_end
endfunction

let &t_SI .= WrapForTmux("\<Esc>[?2004h")
let &t_EI .= WrapForTmux("\<Esc>[?2004l")


"==========================================
" FileType Settings  文件类型设置
"==========================================

" Python 文件的一般设置，比如不要 tab 等
autocmd FileType python set tabstop=4 shiftwidth=4 expandtab ai
autocmd FileType ruby set tabstop=2 shiftwidth=2 softtabstop=2 expandtab ai
autocmd BufRead,BufNew *.md,*.mkd,*.markdown  set filetype=markdown



" 保存文件时删除多余空格
"fun! <SID>StripTrailingWhitespaces()
    "let l = line(".")
    "let c = col(".")
    "%s/\s\+$//e
    "call cursor(l, c)
"endfun
"autocmd FileType c,cpp,java,go,php,javascript,puppet,python,rust,twig,xml,yml,perl autocmd BufWritePre <buffer> :call <SID>StripTrailingWhitespaces()


" 定义函数AutoSetFileHead，自动插入文件头
"autocmd BufNewFile *.sh,*.py exec ":call AutoSetFileHead()"
"function! AutoSetFileHead()
    ""如果文件类型为.sh文件
    "if &filetype == 'sh'
        "call setline(1, "\#!/bin/bash")
    "endif

    ""如果文件类型为python
    "if &filetype == 'python'
        "call setline(1, "\#!/usr/bin/env python")
        "call append(1, "\# encoding: utf-8")
    "endif

    "normal G
    "normal o
    "normal o
"endfunc


" set some keyword to highlight
if has("autocmd")
  " Highlight TODO, FIXME, NOTE, etc.
  if v:version > 701
    autocmd Syntax * call matchadd('Todo',  '\W\zs\(TODO\|FIXME\|CHANGED\|DONE\|XXX\|BUG\|HACK\)')
    autocmd Syntax * call matchadd('Debug', '\W\zs\(NOTE\|INFO\|IDEA\|NOTICE\)')
  endif
endif

"==========================================
" Theme Settings  主题设置
"==========================================

" Set extra options when running in GUI mode
if has("gui_running")
    "set guifont=Monaco:h14
    "if has("gui_gtk2")   "GTK2
        "set guifont=Monaco\ 12,Monospace\ 12
    "endif
    set guioptions-=T
    set guioptions+=e
    set guioptions-=r
    set guioptions-=L
    set guitablabel=%M\ %t
    set showtabline=1
    set linespace=2
    set noimd
    set t_Co=256
endif

" allows cursor change in tmux mode
if exists('$TMUX')
    let &t_SI = "\<Esc>Ptmux;\<Esc>\<Esc>]50;CursorShape=1\x7\<Esc>\\"
    let &t_EI = "\<Esc>Ptmux;\<Esc>\<Esc>]50;CursorShape=0\x7\<Esc>\\"
else
    let &t_SI = "\<Esc>]50;CursorShape=1\x7"
    let &t_EI = "\<Esc>]50;CursorShape=0\x7"
endif

" theme主题
set background=dark
set t_Co=256
colorscheme monokai

" 设置标记一列的背景颜色和数字一行颜色一致
hi! link SignColumn   LineNr
hi! link ShowMarksHLl DiffAdd
hi! link ShowMarksHLu DiffChange

" for error highlight，防止错误整行标红导致看不清
highlight clear SpellBad
highlight SpellBad term=standout ctermfg=1 term=underline cterm=underline
highlight clear SpellCap
highlight SpellCap term=underline cterm=underline
highlight clear SpellRare
highlight SpellRare term=underline cterm=underline
highlight clear SpellLocal
highlight SpellLocal term=underline cterm=underline

" powerline
set rtp+=~/powerline-develop/powerline/bindings/vim
set guifont=DejaVu\ Sans\ Mono\ for\ Powerline:h12
"let g:Powerline_symbols = 'fancy'
"set fillchars+=stl:\ ,stlnc:\
"set term=xterm-256color


" fzf
set rtp+=~/.fzf
map <Leader>f :FZF ~<CR>
