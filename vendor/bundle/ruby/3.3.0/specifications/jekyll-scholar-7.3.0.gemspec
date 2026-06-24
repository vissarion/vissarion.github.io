# -*- encoding: utf-8 -*-
# stub: jekyll-scholar 7.3.0 ruby lib

Gem::Specification.new do |s|
  s.name = "jekyll-scholar".freeze
  s.version = "7.3.0".freeze

  s.required_rubygems_version = Gem::Requirement.new(">= 1.3.6".freeze) if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib".freeze]
  s.authors = ["Sylvester Keil".freeze]
  s.date = "2026-01-06"
  s.description = " Jekyll-Scholar is for all the academic bloggers out there. It is a set of extensions for Jekyll the awesome, blog aware, static site generator; it formats your BibTeX bibliographies for the web using CSL citation styles and generally gives your blog posts citation super-powers.' ".freeze
  s.email = "http://sylvester.keil.or.at".freeze
  s.homepage = "http://github.com/inukshuk/jekyll-scholar".freeze
  s.licenses = ["MIT".freeze]
  s.required_ruby_version = Gem::Requirement.new(">= 3.0".freeze)
  s.rubygems_version = "3.7.2".freeze
  s.summary = "Jekyll extensions for the academic blogger.".freeze

  s.installed_by_version = "3.6.7".freeze

  s.specification_version = 4

  s.add_runtime_dependency(%q<jekyll>.freeze, ["~> 4.0".freeze])
  s.add_runtime_dependency(%q<citeproc-ruby>.freeze, [">= 2.1.6".freeze])
  s.add_runtime_dependency(%q<csl-styles>.freeze, ["~> 2.0".freeze])
  s.add_runtime_dependency(%q<bibtex-ruby>.freeze, ["~> 6.0".freeze])
end
