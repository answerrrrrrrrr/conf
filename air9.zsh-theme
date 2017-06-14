#==========================================
# Air9.zsh-theme
#
# Based on:
#   1. amber.zsh-theme (https://github.com/wbs0829/my-configs)
#   2. bureau.zsh-theme
#   3. http://tw.piliapp.com/symbol/
#
#==========================================

function _git_time_since_commit() {
  if git rev-parse --git-dir > /dev/null 2>&1; then
    # Only proceed if there is actually a commit.
    if [[ $(git log 2>&1 > /dev/null | grep -c "^fatal: bad default revision") == 0 ]]; then
      # Get the last commit.
      last_commit=$(git log --pretty=format:'%at' -1 2> /dev/null)
      now=$(date +%s)
      seconds_since_last_commit=$((now-last_commit))

      # Totals
      minutes=$((seconds_since_last_commit / 60))
      hours=$((seconds_since_last_commit/3600))

      # Sub-hours and sub-minutes
      days=$((seconds_since_last_commit / 86400))
      sub_hours=$((hours % 24))
      sub_minutes=$((minutes % 60))

      if [ $hours -gt 24 ]; then
          commit_age="${days}d"
          #color=$ZSH_THEME_GIT_TIME_SINCE_COMMIT_LONG
      elif [ $minutes -gt 60 ]; then
          commit_age="${sub_hours}h${sub_minutes}m"
          #color=$ZSH_THEME_GIT_TIME_SINCE_COMMIT_MEDIUM
      else
          commit_age="${minutes}m"
          #color=$ZSH_THEME_GIT_TIME_SINCE_COMMIT_SHORT
      fi

      color=$ZSH_THEME_GIT_TIME_SINCE_COMMIT_NEUTRAL
      echo "$color$commit_age%{$reset_color%} "
    fi
  fi
}

ZSH_THEME_GIT_PROMPT_PREFIX="%{$fg[green]%}"
ZSH_THEME_GIT_PROMPT_SUFFIX="%{$reset_color%}"

ZSH_THEME_GIT_PROMPT_DIRTY=" %{$fg[yellow]%}✗ "
ZSH_THEME_GIT_PROMPT_CLEAN=" %{$fg[green]%}✔ "

ZSH_THEME_GIT_PROMPT_UNTRACKED="%{$fg[red]%}┅%{$reset_color%} "
ZSH_THEME_GIT_PROMPT_ADDED="%{$fg[green]%}✚%{$reset_color%} "
ZSH_THEME_GIT_PROMPT_MODIFIED="%{$fg[yellow]%}✎%{$reset_color%} "
ZSH_THEME_GIT_PROMPT_DELETED="%{$fg[yellow]%}━%{$reset_color%} "
ZSH_THEME_GIT_PROMPT_UNMERGED="%{$fg[red]%}§%{$reset_color%} "
ZSH_THEME_GIT_PROMPT_AHEAD="%{$fg[yellow]%}▲%{$reset_color%} "
ZSH_THEME_GIT_PROMPT_BEHIND="%{$fg[yellow]%}▼%{$reset_color%} "

ZSH_THEME_GIT_TIME_SINCE_COMMIT_SHORT="%{$fg[green]%}"
ZSH_THEME_GIT_TIME_SINCE_COMMIT_MEDIUM="%{$fg[blue]%}"
ZSH_THEME_GIT_TIME_SINCE_COMMIT_LONG="%{$fg[yellow]%}"
ZSH_THEME_GIT_TIME_SINCE_COMMIT_NEUTRAL="%{$fg[yellow]%}"



if [[ $USER == "root" ]]; then
  CARETCOLOR="white"
else
  CARETCOLOR="green"
fi

local _color_prompt="%{$fg[${CARETCOLOR}]%}%(?..%{$fg[red]%})>%{$reset_color%}"
local _current_dir="%{$fg_bold[white]%}%~%{$reset_color%} "
local _return_status="%{$fg[red]%}%(?..⍉ )%{$reset_color%}"
local _return_code_neat="%{$fg[red]%}%(?..%? )%{$reset_color%}"
local _return_code_pretty="%{$fg[red]%}%(?..%?↵ )%{$reset_color%}"
local _return_code_always="%{$fg[red]%}%? %{$reset_color%}"
local _hist_no="%{$fg[yellow]%}%h%{$reset_color%} "
local _date_time="%{$fg_bold[white]%}%*%{$reset_color%}"

function _user_host() {
  echo "%{$fg[green]%}%n%{$reset_color%}@%m "
}

function _ruby_version() {
  if {echo $fpath | grep -q "plugins/rvm"}; then
    echo "%{$fg[yellow]%}$(rvm_prompt_info)%{$reset_color%} "
  fi
}



local PROMPT_L1='${_date_time} $(_user_host)${_current_dir}$(git_prompt_info)$(_git_time_since_commit)$(git_prompt_status)$(git_prompt_ahead)$(_ruby_version)'

local PROMPT_L2='${_return_code_neat}${_color_prompt} '

setopt prompt_subst
air9_precmd () {
        print
        print -rP "$PROMPT_L1"
}

PROMPT="${PROMPT_L2}"
RPROMPT=''

autoload -U add-zsh-hook
add-zsh-hook precmd air9_precmd
