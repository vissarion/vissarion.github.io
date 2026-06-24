---
layout: page
permalink: /software/
title: Software
description: Open-source software projects and libraries.
nav: false
nav_order: 3
---

## Featured Projects

<div class="projects" markdown="1">

{% for project in site.data.software_projects %}

### {{ project.title }}
{: .project-title}

{{ project.description }}

<p class="post-meta">
  {% if project.repo_url %}<a href="{{ project.repo_url }}" target="_blank" rel="noopener noreferrer"><i class="fa-solid fa-code"></i> Repository</a>{% endif %}
  {% if project.docs_url %} &nbsp;|&nbsp; <a href="{{ project.docs_url }}" target="_blank" rel="noopener noreferrer"><i class="fa-solid fa-book"></i> Docs</a>{% endif %}
  {% if project.paper_url %} &nbsp;|&nbsp; <a href="{{ project.paper_url }}"><i class="fa-solid fa-file-lines"></i> Paper</a>{% endif %}
  {% if project.language %} &nbsp;&middot;&nbsp; <span class="badge">{{ project.language }}</span>{% endif %}
</p>

{% endfor %}

</div>

---

{% if site.data.repositories.github_users %}

## GitHub Activity

<div class="repositories d-flex flex-wrap flex-md-row flex-column justify-content-between align-items-center">
  {% for user in site.data.repositories.github_users %}
    {% include repository/repo_user.liquid username=user %}
  {% endfor %}
</div>

{% if site.data.repositories.github_repos %}

### Repositories

<div class="repositories d-flex flex-wrap flex-md-row flex-column justify-content-between align-items-center">
  {% for repo in site.data.repositories.github_repos %}
    {% include repository/repo.liquid repository=repo %}
  {% endfor %}
</div>
{% endif %}

{% endif %}
